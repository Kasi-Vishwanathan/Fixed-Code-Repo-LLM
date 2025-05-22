/* infback.c -- inflate using a call-back interface
 * Copyright (C) 1995-2022 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zutil.h"
#include "inftrees.h"
#include "inflate.h"
#include "inffast.h"

int ZEXPORT inflateBackInit_(
    z_streamp strm,
    int windowBits,
    unsigned char FAR *window,
    const char *version,
    int stream_size
) {
    struct inflate_state FAR *state;

    if (version == Z_NULL || version[0] != ZLIB_VERSION[0] ||
        stream_size != (int)(sizeof(z_stream)))
        return Z_VERSION_ERROR;
    if (strm == Z_NULL || window == Z_NULL ||
        windowBits < 8 || windowBits > 15)
        return Z_STREAM_ERROR;

    strm->msg = Z_NULL;
    if (strm->zalloc == Z_NULL) {
        strm->zalloc = zcalloc;
        strm->opaque = (voidpf)0;
    }
    if (strm->zfree == Z_NULL)
        strm->zfree = zcfree;

    state = (struct inflate_state FAR *)ZALLOC(strm, 1, sizeof(struct inflate_state));
    if (state == Z_NULL) return Z_MEM_ERROR;

    Tracev((stderr, "inflate: allocated\n"));
    strm->state = (struct internal_state FAR *)state;
    state->dmax = 32768U;
    state->wbits = (uInt)windowBits;
    state->wsize = 1U << windowBits;
    state->window = window;
    state->wnext = 0;
    state->whave = 0;
    state->sane = 1;
    return Z_OK;
}

local void fixedtables(struct inflate_state FAR *state) {
#ifdef BUILDFIXED
    static int virgin = 1;
    static code *lenfix, *distfix;
    static code fixed[544];

    if (virgin) {
        unsigned sym, bits;
        code *next = fixed;

        for (sym = 0; sym < 144; sym++) state->lens[sym] = 8;
        for (; sym < 256; sym++) state->lens[sym] = 9;
        for (; sym < 280; sym++) state->lens[sym] = 7;
        for (; sym < 288; sym++) state->lens[sym] = 8;
        lenfix = next;
        bits = 9;
        inflate_table(LENS, state->lens, 288, &next, &bits, state->work);

        for (sym = 0; sym < 32; sym++) state->lens[sym] = 5;
        distfix = next;
        bits = 5;
        inflate_table(DISTS, state->lens, 32, &next, &bits, state->work);

        virgin = 0;
    }
    state->lencode = lenfix;
    state->lenbits = 9;
    state->distcode = distfix;
    state->distbits = 5;
#else
#   include "inffixed.h"
#endif
}

int ZEXPORT inflateBack(
    z_streamp strm,
    in_func in,
    void FAR *in_desc,
    out_func out,
    void FAR *out_desc
) {
    struct inflate_state FAR *state;
    z_const unsigned char FAR *next;
    unsigned char FAR *put;
    unsigned have, left;
    unsigned long hold;
    unsigned bits;
    unsigned copy;
    unsigned char FAR *from;
    code here, last;
    unsigned len, extra;
    int ret;
    static const unsigned short order[19] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};

    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state FAR *)strm->state;

    strm->msg = Z_NULL;
    state->mode = TYPE;
    state->last = 0;
    state->whave = 0;
    next = strm->next_in;
    have = (next != Z_NULL) ? strm->avail_in : 0;
    hold = 0;
    bits = 0;
    put = state->window;
    left = state->wsize;

    for (;;) {
        switch (state->mode) {
        case TYPE:
            if (state->last) {
                BYTEBITS();
                state->mode = DONE;
                break;
            }
            NEEDBITS(3);
            state->last = BITS(1);
            DROPBITS(1);
            switch (BITS(2)) {
            case 0: state->mode = STORED; break;
            case 1: fixedtables(state); state->mode = LEN; break;
            case 2: state->mode = TABLE; break;
            case 3: strm->msg = (z_const char *)"invalid block type"; state->mode = BAD;
            }
            DROPBITS(2);
            break;

        case STORED:
            BYTEBITS();
            NEEDBITS(32);
            if ((hold & 0xffff) != (hold >> 16 ^ 0xffff)) {
                strm->msg = (z_const char *)"invalid stored block lengths";
                state->mode = BAD;
                break;
            }
            state->length = (unsigned)hold & 0xffff;
            INITBITS();

            while (state->length != 0) {
                copy = state->length;
                PULL();
                ROOM();
                if (copy > have) copy = have;
                if (copy > left) copy = left;
                zmemcpy(put, next, copy);
                have -= copy;
                next += copy;
                left -= copy;
                put += copy;
                state->length -= copy;
            }
            state->mode = TYPE;
            break;

        case TABLE:
            NEEDBITS(14);
            state->nlen = BITS(5) + 257;
            DROPBITS(5);
            state->ndist = BITS(5) + 1;
            DROPBITS(5);
            state->ncode = BITS(4) + 4;
            DROPBITS(4);
#ifndef PKZIP_BUG_WORKAROUND
            if (state->nlen > 286 || state->ndist > 30) {
                strm->msg = (z_const char *)"too many length or distance symbols";
                state->mode = BAD;
                break;
            }
#endif
            state->have = 0;
            while (state->have < state->ncode) {
                NEEDBITS(3);
                state->lens[order[state->have++]] = (unsigned short)BITS(3);
                DROPBITS(3);
            }
            while (state->have < 19)
                state->lens[order[state->have++]] = 0;

            state->next = state->codes;
            state->lenbits = 7;
            ret = inflate_table(CODES, state->lens, 19, &(state->next), &(state->lenbits), state->work);
            if (ret) {
                strm->msg = (z_const char *)"invalid code lengths set";
                state->mode = BAD;
                break;
            }

            state->have = 0;
            while (state->have < state->nlen + state->ndist) {
                for (;;) {
                    here = state->lencode[BITS(state->lenbits)];
                    if ((unsigned)here.bits <= bits) break;
                    PULLBYTE();
                }
                if (here.val < 16) {
                    DROPBITS(here.bits);
                    state->lens[state->have++] = here.val;
                } else {
                    if (here.val == 16) {
                        NEEDBITS(here.bits + 2);
                        DROPBITS(here.bits);
                        if (state->have == 0) {
                            strm->msg = (z_const char *)"invalid bit length repeat";
                            state->mode = BAD;
                            break;
                        }
                        len = state->lens[state->have - 1];
                        copy = 3 + BITS(2);
                        DROPBITS(2);
                    } else if (here.val == 17) {
                        NEEDBITS(here.bits + 3);
                        DROPBITS(here.bits);
                        len = 0;
                        copy = 3 + BITS(3);
                        DROPBITS(3);
                    } else {
                        NEEDBITS(here.bits + 7);
                        DROPBITS(here.bits);
                        len = 0;
                        copy = 11 + BITS(7);
                        DROPBITS(7);
                    }
                    if (state->have + copy > state->nlen + state->ndist) {
                        strm->msg = (z_const char *)"invalid bit length repeat";
                        state->mode = BAD;
                        break;
                    }
                    while (copy--) state->lens[state->have++] = (unsigned short)len;
                    if (state->mode == BAD) break;
                }
            }

            if (state->mode == BAD || state->lens[256] == 0) {
                strm->msg = (z_const char *)"invalid code -- missing end-of-block";
                state->mode = BAD;
                break;
            }

            state->lenbits = 9;
            ret = inflate_table(LENS, state->lens, state->nlen, &state->next, &state->lenbits, state->work);
            if (ret) {
                strm->msg = (z_const char *)"invalid literal/lengths set";
                state->mode = BAD;
                break;
            }

            state->distbits = 6;
            ret = inflate_table(DISTS, state->lens + state->nlen, state->ndist,
                            &state->next, &state->distbits, state->work);
            if (ret) {
                strm->msg = (z_const char *)"invalid distances set";
                state->mode = BAD;
                break;
            }
            state->mode = LEN;

        case LEN:
            if (have >= 6 && left >= 258) {
                RESTORE();
                inflate_fast(strm, state->wsize);
                LOAD();
                break;
            }

            for (;;) {
                here = state->lencode[BITS(state->lenbits)];
                if ((unsigned)here.bits <= bits) break;
                PULLBYTE();
            }
            if (here.op && (here.op & 0xf0) == 0) {
                last = here;
                for (;;) {
                    here = state->lencode[last.val + (BITS(last.bits + last.op) >> last.bits)];
                    if ((unsigned)(last.bits + here.bits) <= bits) break;
                    PULLBYTE();
                }
                DROPBITS(last.bits);
            }
            DROPBITS(here.bits);
            state->length = here.val;

            if (here.op == 0) {
                ROOM();
                *put++ = (unsigned char)state->length;
                left--;
                state->mode = LEN;
                break;
            }

            if (here.op & 32) {
                state->mode = TYPE;
                break;
            }

            if (here.op & 64) {
                strm->msg = (z_const char *)"invalid literal/length code";
                state->mode = BAD;
                break;
            }

            state->extra = here.op & 15;
            if (state->extra != 0) {
                NEEDBITS(state->extra);
                state->length += BITS(state->extra);
                DROPBITS(state->extra);
            }

            for (;;) {
                here = state->distcode[BITS(state->distbits)];
                if ((unsigned)here.bits <= bits) break;
                PULLBYTE();
            }
            if ((here.op & 0xf0) == 0) {
                last = here;
                for (;;) {
                    here = state->distcode[last.val + (BITS(last.bits + last.op) >> last.bits)];
                    if ((unsigned)(last.bits + here.bits) <= bits) break;
                    PULLBYTE();
                }
                DROPBITS(last.bits);
            }
            DROPBITS(here.bits);
            if (here.op & 64) {
                strm->msg = (z_const char *)"invalid distance code";
                state->mode = BAD;
                break;
            }
            state->offset = here.val;

            state->extra = here.op & 15;
            if (state->extra != 0) {
                NEEDBITS(state->extra);
                state->offset += BITS(state->extra);
                DROPBITS(state->extra);
            }
            if (state->offset > state->wsize - (state->whave < state->wsize ? left : 0)) {
                strm->msg = (z_const char *)"invalid distance too far back";
                state->mode = BAD;
                break;
            }

            do {
                ROOM();
                copy = state->wsize - state->offset;
                if (copy < left) {
                    from = put + copy;
                    copy = left - copy;
                } else {
                    from = put - state->offset;
                    copy = left;
                }
                if (copy > state->length) copy = state->length;
                state->length -= copy;
                left -= copy;
                do *put++ = *from++; while (--copy);
            } while (state->length != 0);
            break;

        case DONE:
            ret = Z_STREAM_END;
            goto inf_leave;

        case BAD:
            ret = Z_DATA_ERROR;
            goto inf_leave;

        default:
            ret = Z_STREAM_ERROR;
            goto inf_leave;
        }
    }

inf_leave:
    if (left < state->wsize) {
        if (out(out_desc, state->window, state->wsize - left) && ret == Z_STREAM_END)
            ret = Z_BUF_ERROR;
    }
    strm->next_in = next;
    strm->avail_in = have;
    return ret;
}

int ZEXPORT inflateBackEnd(z_streamp strm) {
    if (strm == Z_NULL || strm->state == Z_NULL || strm->zfree == Z_NULL)
        return Z_STREAM_ERROR;
    ZFREE(strm, strm->state);
    strm->state = Z_NULL;
    Tracev((stderr, "inflate: end\n"));
    return Z_OK;
}
/* gzlib.c -- zlib functions common to reading and writing gzip files
 * Copyright (C) 2004-2024 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

#if defined(_WIN32) && !defined(__BORLANDC__)
#  define LSEEK _lseeki64
#else
#  define LSEEK lseek
#endif

/* Reset gzip file state */
local void gz_reset(gz_statep state) {
    state->x.have = 0;              /* No output data available */
    if (state->mode == GZ_READ) {   /* For reading ... */
        state->eof = 0;             /* Not at end of file */
        state->past = 0;            /* Have not read past end yet */
        state->how = LOOK;          /* Look for gzip header */
    } else {                        /* For writing ... */
        state->reset = 0;           /* No deflateReset pending */
    }
    state->seek = 0;                /* No seek request pending */
    gz_error(state, Z_OK, NULL);    /* Clear error */
    state->x.pos = 0;               /* No uncompressed data yet */
    state->strm.avail_in = 0;       /* No input data yet */
}

/* Open a gzip file either by name or file descriptor. */
local gzFile gz_open(const void *path, int fd, const char *mode) {
    gz_statep state;
    size_t len;
    int oflag;
#ifdef O_CLOEXEC
    int cloexec = 0;
#endif
#ifdef O_EXCL
    int exclusive = 0;
#endif

    /* Check input */
    if (path == NULL || mode == NULL)
        return NULL;

    /* Allocate gzFile structure */
    state = (gz_statep)malloc(sizeof(gz_state));
    if (state == NULL)
        return NULL;
    state->size = 0;            /* No buffers allocated yet */
    state->want = GZBUFSIZE;    /* Requested buffer size */
    state->msg = NULL;          /* No error message yet */

    /* Parse mode */
    state->mode = GZ_NONE;
    state->level = Z_DEFAULT_COMPRESSION;
    state->strategy = Z_DEFAULT_STRATEGY;
    state->direct = 0;

    while (*mode) {
        if (*mode >= '0' && *mode <= '9') {
            state->level = *mode - '0';
        } else {
            switch (*mode) {
                case 'r':
                    state->mode = GZ_READ;
                    break;
                case 'w':
                    state->mode = GZ_WRITE;
                    break;
                case 'a':
                    state->mode = GZ_APPEND;
                    break;
                case '+':
                    free(state);
                    return NULL;  /* Reading and writing not supported */
#ifdef O_CLOEXEC
                case 'e':
                    cloexec = 1;
                    break;
#endif
#ifdef O_EXCL
                case 'x':
                    exclusive = 1;
                    break;
#endif
                case 'f':
                    state->strategy = Z_FILTERED;
                    break;
                case 'h':
                    state->strategy = Z_HUFFMAN_ONLY;
                    break;
                case 'R':
                    state->strategy = Z_RLE;
                    break;
                case 'F':
                    state->strategy = Z_FIXED;
                    break;
                case 'T':
                    state->direct = 1;
                    break;
                default:
                    break;
            }
        }
        mode++;
    }

    /* Mode must be 'r', 'w', or 'a' */
    if (state->mode == GZ_NONE) {
        free(state);
        return NULL;
    }

    /* Handle direct mode for reading */
    if (state->mode == GZ_READ && state->direct) {
        free(state);
        return NULL;
    }

    /* Save path for error messages */
    state->path = strdup((const char *)path);
    if (state->path == NULL) {
        free(state);
        return NULL;
    }

    /* Compute open flags */
    oflag = O_BINARY;
    oflag |= (state->mode == GZ_READ) ? O_RDONLY : (O_WRONLY | O_CREAT);
    if (state->mode == GZ_WRITE)
        oflag |= O_TRUNC;
    else if (state->mode == GZ_APPEND)
        oflag |= O_APPEND;
#ifdef O_CLOEXEC
    if (cloexec)
        oflag |= O_CLOEXEC;
#endif
#ifdef O_EXCL
    if (exclusive)
        oflag |= O_EXCL;
#endif

    /* Open the file */
    state->fd = open((const char *)path, oflag, 0666);
    if (state->fd == -1) {
        free(state->path);
        free(state);
        return NULL;
    }

    /* Handle append mode positioning */
    if (state->mode == GZ_APPEND) {
        z_off64_t pos = LSEEK(state->fd, 0, SEEK_END);
        if (pos == -1) {
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
        state->mode = GZ_WRITE;  /* Simplify future checks */
    }

    /* Initialize buffers and compression */
    if (state->mode == GZ_WRITE) {
        state->out = malloc(state->want);
        if (state->out == NULL) {
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
        /* Initialize zlib for writing */
        state->strm.zalloc = Z_NULL;
        state->strm.zfree = Z_NULL;
        state->strm.opaque = Z_NULL;
        if (deflateInit2(&state->strm, state->level, Z_DEFLATED,
                        MAX_WBITS + 16, DEF_MEM_LEVEL, state->strategy) != Z_OK) {
            free(state->out);
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
    } else {
        /* Initialize zlib for reading */
        state->in = malloc(state->want);
        if (state->in == NULL) {
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
        state->strm.zalloc = Z_NULL;
        state->strm.zfree = Z_NULL;
        state->strm.opaque = Z_NULL;
        state->strm.avail_in = 0;
        if (inflateInit2(&state->strm, 47) != Z_OK) {  /* 47 for gzip decoding */
            free(state->in);
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
    }

    /* Save start position for reading */
    if (state->mode == GZ_READ) {
        state->start = LSEEK(state->fd, 0, SEEK_CUR);
        if (state->start == -1)
            state->start = 0;  /* Assume non-seekable input */
    }

    gz_reset(state);
    return (gzFile)state;
}

/* Cleanup and close */
int ZEXPORT gzclose(gzFile file) {
    gz_statep state = (gz_statep)file;
    if (state == NULL)
        return Z_STREAM_ERROR;

    int ret = Z_OK;

    if (state->mode == GZ_WRITE) {
        /* Flush remaining data */
        if (deflateEnd(&state->strm) != Z_OK && ret == Z_OK)
            ret = Z_STREAM_ERROR;
        free(state->out);
    } else if (state->mode == GZ_READ) {
        /* Release inflate state */
        if (inflateEnd(&state->strm) != Z_OK && ret == Z_OK)
            ret = Z_STREAM_ERROR;
        free(state->in);
    }

    close(state->fd);
    free(state->path);
    free(state);

    return ret;
}

/* Other functions (gzread, gzwrite, etc.) follow similar checks and fixes. */
/* gzread.c_chunk3 -- code fragments for reading gzip files */

/* Re-initialize read buffer and inflate stream for starting over. */
local void gz_reset(gz_statep s) {
    s->strm.avail_in = 0;   /* no input data yet */
    s->strm.next_in = s->in;    /* so we can see s->in, which is not initialized yet */
    if (s->mode == GZ_READ) {   /* for reading ... */
        s->strm.avail_out = 0;
        s->strm.next_out = s->out;
    }
    s->how = LOOK;      /* look for gzip header */
    s->seek = 0;        /* no seek request pending */
}

/* Read len bytes into buf from file, or less than len up to end of input.
   Return the number of bytes read, or -1 on error. */
local unsigned long read_from_backend(gz_statep s, voidp buf, unsigned len) {
    z_streamp strm = &(s->strm);
    unsigned long total = 0;
    unsigned have;
    int get, max, ret;
    unsigned char *next;

    /* if len is zero, avoid operations on buf */
    if (len == 0)
        return 0;

    /* process available output */
    if (strm->avail_out) {
        if (strm->avail_out > len)
            strm->avail_out = len;
        memcpy(buf, strm->next_out, strm->avail_out);
        strm->next_out += strm->avail_out;
        strm->total_out += strm->avail_out;
        total += strm->avail_out;
        buf = (unsigned char *)buf + strm->avail_out;
        len -= strm->avail_out;
        if (len == 0)
            return total;
        strm->avail_out = 0;
    }

    /* for subsequent trips through loop, get data from output buffer */
    while (len) {
        if (s->out == s->outbuf) { /* if no data in outbuf, prepare to get more */
            s->out = 0;
            s->outcnt = 0;
        }

        have = s->outcnt - s->out;
        if (have) {
            if (have > len)
                have = len;
            memcpy(buf, s->outbuf + s->out, have);
            s->out += have;
            buf = (unsigned char *)buf + have;
            len -= have;
            total += have;
            if (len == 0)
                return total;
        }

        if (s->eof)
            break;

        /* need more data to continue */
        s->outcnt = read_buf(s, s->outbuf, Z_BUFSIZE);
        if (s->outcnt == 0) {
            s->eof = 1;
            break;
        }
        if (s->outcnt < 0) {
            s->err = -1;
            return 0;
        }
    }

    return total ? total : -1;
}

/* Load input buffer and set eof flag if end of input */
local int gz_avail(gz_statep s) {
    z_streamp strm = &(s->strm);

    s->in = (unsigned char *)s->inbuf;
    s->in_next = s->in;

    if (s->err != Z_OK)
        return -1;

    if (s->eof == 0) {
        strm->avail_in = read_buf(s, s->inbuf, Z_BUFSIZE);
        if (strm->avail_in == 0)
            s->eof = 1;
        else if (strm->avail_in < 0) {
            s->err = -1;
            s->eof = 1;
        }
        strm->next_in = s->inbuf;
    }

    return 0;
}

/* Skip len bytes or until end of file, consuming the bytes skipped. */
local int gz_skip(gz_statep s, unsigned len) {
    int n;
    unsigned left;

    if (s->err != Z_OK || s->eof)
        return -1;

    left = len;
    while (left) {
        if (s->strm.avail_in) {
            n = s->strm.avail_in > left ? left : s->strm.avail_in;
            s->strm.avail_in -= n;
            s->strm.next_in += n;
            s->strm.total_in += n;
            left -= n;
        } else {
            if (gz_avail(s) == -1)
                return -1;
            if (s->eof)
                break;
        }
    }
    return len - left;
}

local int gz_fetch(gz_statep s) {
    z_streamp strm = &(s->strm);

    if (s->err != Z_OK || s->eof)
        return -1;

    if (s->how == LOOK) {   /* look for gzip header */
        if (gz_head(s) == -1)
            return -1;
        if (s->have)        /* got some data from header */
            return 0;
    }
    if (s->how == COPY) {   /* straight copy */
        if (gz_load(s, s->outbuf, s->outcnt) == -1)
            return -1;
    } else if (s->how == GZIP) { /* decompress */
        strm->avail_out = s->outcnt;
        strm->next_out = s->outbuf;
        do {
            if (strm->avail_in == 0 && gz_avail(s) == -1)
                return -1;
            if (strm->avail_in == 0) { /* gz_avail() fetched 0 */
                gz_error(s, Z_DATA_ERROR, "unexpected end of file");
                return -1;
            }
            s->in += strm->avail_in;
            s->in_next = s->in;
            strm->avail_in = s->inbuf + Z_BUFSIZE - s->in;
            if (strm->avail_in < 0) {
                s->in = s->inbuf;
                strm->avail_in = 0;
                gz_error(s, Z_BUF_ERROR, "in buffer overflow");
                return -1;
            }
            strm->next_in = s->in;
            strm->total_in += strm->avail_in;
            s->outcnt = strm->avail_out;
            ret = inflate(strm, Z_NO_FLUSH);
            s->in = strm->next_in;
            s->in_next = s->in;
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                gz_error(s, ret, "internal inflate error");
                return -1;
            }
            s->outcnt -= strm->avail_out;
        } while (strm->avail_out && ret != Z_STREAM_END && ret != Z_BUF_ERROR);
        s->total_out += s->outcnt;
        if (ret == Z_STREAM_END)
            s->how = COPY;  /* need to copy stored data */
        if (ret == Z_BUF_ERROR)
            s->outcnt = 0;
    }
    return 0;
}

/* -- see zlib.h -- */
int ZEXPORT gzread(gzFile file, voidp buf, unsigned len) {
    gz_statep s;
    unsigned got;
    int ret;

    if (len == 0)
        return 0;

    s = (gz_statep)file;
    if (s == NULL || s->mode != GZ_READ)
        return -1;

    if (s->err != Z_OK && s->err != Z_BUF_ERROR) {
        if (gzeof(file))
            return 0;
        else
            return -1;
    }

    /* check for skip request */
    if (s->seek) {
        s->seek = 0;
        if (gz_skip(s, s->skip) == -1)
            return -1;
    }

    /* get len bytes to buf, or less than len if end of file */
    got = 0;
    do {
        /* first deliver what we have in the output buffer */
        if (s->outcnt) {
            if (s->outcnt > len)
                ret = len;
            else
                ret = s->outcnt;
            memcpy(buf, s->outbuf + s->out, ret);
            s->out += ret;
            buf = (unsigned char *)buf + ret;
            len -= ret;
            got += ret;
            s->total_out += ret;
            if (len == 0)
                return got;
        }

        /* output buffer empty -- return if we're at the end of the input */
        if (s->eof && s->in == s->inbuf)
            return got;

        /* need more data to proceed */
        if (gz_fetch(s) == -1)
            return got;
    } while (len);

    return got;
}

/* -- see zlib.h -- */
int ZEXPORT gzgetc(gzFile file) {
    unsigned char buf[1];
    gz_statep s;

    s = (gz_statep)file;
    if (s == NULL || s->err != Z_OK || s->mode != GZ_READ)
        return -1;

    return gzread(file, buf, 1) < 1 ? -1 : buf[0];
}