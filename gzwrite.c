/* gzread.c -- zlib functions for reading gzip files
 * Copyright (C) 2004-2017 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

/* Use read() to load a buffer -- return -1 on error, otherwise 0.  Read from
   state->fd, and update state->eof, state->err, and state->msg as appropriate.
   This function needs to loop on read(), since read() is not guaranteed to
   read the number of bytes requested, depending on the type of descriptor. */
local int gz_load(gz_statep state, unsigned char *buf, unsigned len, unsigned *have) {
    int ret;
    size_t get, max = (size_t)((unsigned)-1 >> 2) + 1;

    *have = 0;
    do {
        get = len - *have;
        if (get > max)
            get = max;
        ret = read(state->fd, buf + *have, get);
        if (ret <= 0)
            break;
        *have += (unsigned)ret;
    } while (*have < len);
    if (ret < 0) {
        gz_error(state, Z_ERRNO, zstrerror());
        return -1;
    }
    if (ret == 0)
        state->eof = 1;
    return 0;
}

/* Load up input buffer and set eof flag if last data loaded -- return -1 on
   error, 0 otherwise.  Note that the eof flag is set when the end of the input
   file is reached, even though there may be unused data in the buffer.  Once
   that data has been used, no more attempts will be made to read the file.
   If strm->avail_in != 0, then the current data is moved to the beginning of
   the input buffer, and then the remainder of the buffer is loaded with the
   available data from the input file. */
local int gz_avail(gz_statep state) {
    unsigned got;
    z_streamp strm = &(state->strm);

    if (state->err != Z_OK && state->err != Z_BUF_ERROR)
        return -1;
    if (state->eof == 0) {
        if (strm->avail_in) {       /* copy what's there to the start */
            memmove(state->in, strm->next_in, strm->avail_in);
            strm->next_in = state->in;
        }
        if (gz_load(state, state->in + strm->avail_in,
                    state->size - strm->avail_in, &got) == -1)
            return -1;
        strm->avail_in += got;
    }
    return 0;
}

/* Look for gzip header, set up for inflate or copy.  state->x.have must be 0.
   If this is the first time in, allocate required memory.  state->how will be
   left unchanged if there is no more input data available, will be set to COPY
   if there is no gzip header and direct copying will be performed, or it will
   be set to GZIP for decompression.  If direct copying, then leftover input
   data from the input buffer will be copied to the output buffer.  In that
   case, all further file reads will be directly to either the output buffer or
   a user buffer.  If decompressing, the inflate state will be initialized.
   gz_look() will return 0 on success or -1 on failure. */
local int gz_look(gz_statep state) {
    z_streamp strm = &(state->strm);

    /* allocate read buffers and inflate memory */
    if (state->size == 0) {
        /* allocate buffers */
        state->in = (unsigned char *)malloc(state->want);
        state->out = (unsigned char *)malloc(state->want << 1);
        if (state->in == NULL || state->out == NULL) {
            free(state->out);
            free(state->in);
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }
        state->size = state->want;

        /* allocate inflate memory */
        state->strm.zalloc = Z_NULL;
        state->strm.zfree = Z_NULL;
        state->strm.opaque = Z_NULL;
        state->strm.avail_in = 0;
        state->strm.next_in = Z_NULL;
        if (inflateInit2(&(state->strm), 15 + 16) != Z_OK) {    /* gunzip */
            free(state->out);
            free(state->in);
            state->size = 0;
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }
    }

    /* get at least the magic bytes in the input buffer */
    if (strm->avail_in < 2) {
        if (gz_avail(state) == -1)
            return -1;
        if (strm->avail_in == 0)
            return 0;
    }

    /* look for gzip magic bytes -- if there, do gzip decoding (note: there is
       a logical dilemma here when considering the case of a partially written
       gzip file, to wit, if a single 31 byte is written, then we cannot tell
       whether this is a single-byte file, or just a partially written gzip
       file -- for here we assume that if a gzip file is being written, then
       the header will be written in a single operation, so that reading a
       single byte is sufficient indication that it is not a gzip file) */
    if (strm->avail_in > 1 &&
            strm->next_in[0] == 31 && strm->next_in[1] == 139) {
        inflateReset(strm);
        state->how = GZIP;
        state->direct = 0;
        return 0;
    }

    /* no gzip header -- if we were decoding gzip before, then this is trailing
       garbage.  Ignore the trailing garbage and finish. */
    if (state->direct == 0) {
        strm->avail_in = 0;
        state->eof = 1;
        state->x.have = 0;
        return 0;
    }

    /* doing raw i/o, copy any leftover input to output -- this assumes that
       the output buffer is larger than the input buffer, which also assures
       space for gzungetc() */
    state->x.next = state->out;
    memcpy(state->x.next, strm->next_in, strm->avail_in);
    state->x.have = strm->avail_in;
    strm->avail_in = 0;
    state->how = COPY;
    state->direct = 1;
    return 0;
}

/* Decompress from input to the provided next_out and avail_out in the state.
   On return, state->x.have and state->x.next point to the just decompressed
   data.  If the gzip stream completes, state->how is reset to LOOK to look for
   the next gzip stream or raw data, once state->x.have is depleted.  Returns 0
   on success, -1 on failure. */
local int gz_decomp(gz_statep state) {
    int ret = Z_OK;
    unsigned had;
    z_streamp strm = &(state->strm);

    /* fill output buffer up to end of deflate stream */
    had = strm->avail_out;
    do {
        /* get more input for inflate() */
        if (strm->avail_in == 0 && gz_avail(state) == -1)
            return -1;
        if (strm->avail_in == 0) {
            gz_error(state, Z_BUF_ERROR, "unexpected end of file");
            break;
        }

        /* decompress and handle errors */
        ret = inflate(strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_NEED_DICT) {
            gz_error(state, Z_STREAM_ERROR,
                     "internal error: inflate stream corrupt");
            return -1;
        }
        if (ret == Z_MEM_ERROR) {
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }
        if (ret == Z_DATA_ERROR) {              /* deflate stream invalid */
            gz_error(state, Z_DATA_ERROR,
                     strm->msg == NULL ? "compressed data error" : strm->msg);
            return -1;
        }
    } while (strm->avail_out && ret != Z_STREAM_END && ret != Z_BUF_ERROR);

    /* update available output */
    state->x.have = had - strm->avail_out;
    state->x.next = strm->next_out - state->x.have;

    /* if the gzip stream completed successfully, look for another */
    if (ret == Z_STREAM_END)
        state->how = LOOK;

    /* good decompression */
    return 0;
}

/* Fetch data and put it in the output buffer.  Assumes state->x.have is 0.
   Data is either copied from the input file or decompressed from the input
   file depending on state->how.  If state->how is LOOK, then a gzip header is
   looked for to determine whether to copy or decompress.  Returns -1 on error,
   otherwise 0.  gz_fetch() will leave state->how as COPY or GZIP unless the
   end of the input file has been reached and all data has been processed.  */
local int gz_fetch(gz_statep state) {
    z_streamp strm = &(state->strm);

    do {
        switch(state->how) {
        case LOOK:      /* -> LOOK, COPY (only if never GZIP), or GZIP */
            if (gz_look(state) == -1)
                return -1;
            if (state->how == LOOK)
                return 0;
            break;
        case COPY:      /* -> COPY */
            if (gz_load(state, state->out, state->size << 1, &(state->x.have))
                    == -1)
                return -1;
            state->x.next = state->out;
            return 0;
        case GZIP:      /* -> GZIP or LOOK (if end of gzip stream) */
            strm->avail_out = state->size << 1;
            strm->next_out = state->out;
            if (gz_decomp(state) == -1)
                return -1;
        }
    } while (state->x.have == 0 && (!state->eof || strm->avail_in));
    return 0;
}

/* Skip len uncompressed bytes of output. Return -1 on error, 0 on success. */
local int gz_skip(gz_statep state, z_off64_t len) {
    unsigned n;

    /* skip over len bytes or reach end-of-file, whichever comes first */
    while (len > 0) {
        /* skip over whatever is in output buffer */
        if (state->x.have) {
            n = GT_OFF(state->x.have) || (z_off64_t)state->x.have > len ?
                (unsigned)len : state->x.have;
            state->x.have -= n;
            state->x.next += n;
            state->x.pos += n;
            len -= n;
        }

        /* output buffer empty -- return if we're at the end of the input */
        else if (state->eof && strm->avail_in == 0)
            break;

        /* need more data to skip -- load up output buffer */
        else {
            if (gz_fetch(state) == -1)
                return -1;
        }
    }
    return len == 0 ? 0 : -1;
}

/* Read len bytes into buf from file, or less than len up to the end of the
   input.  Return the number of bytes read.  If zero is returned, either the
   end of file was reached, or there was an error.  state->err must be
   consulted in that case to determine which. */
local z_size_t gz_read(gz_statep state, voidp buf, z_size_t len) {
    z_size_t got;
    unsigned n;

    /* if len is zero, avoid unnecessary operations */
    if (len == 0)
        return 0;

    /* process a skip request */
    if (state->seek) {
        state->seek = 0;
        if (gz_skip(state, state->skip) == -1)
            return 0;
    }

    /* get len bytes to buf, or less than len if at the end */
    got = 0;
    do {
        /* set n to the maximum amount of len that fits in an unsigned int */
        n = (unsigned)-1;
        if (n > len)
            n = (unsigned)len;

        /* first just try copying data from the output buffer */
        if (state->x.have) {
            if (state->x.have < n)
                n = state->x.have;
            memcpy(buf, state->x.next, n);
            state->x.next += n;
            state->x.have -= n;
        }

        /* output buffer empty -- return if we're at the end of the input */
        else if (state->eof && strm->avail_in == 0) {
            state->past = 1;        /* tried to read past end */
            break;
        }

        /* need output data -- for small len or new stream load up our output
           buffer */
        else if (state->how == LOOK || n < (state->size << 1)) {
            /* get more output, looking for header if required */
            if (gz_fetch(state) == -1)
                return 0;
            continue;       /* no progress yet -- go back to copy above */
            /* the copy above assures that we will leave with space in the
               output buffer, allowing at least one gzungetc() to succeed */
        }

        /* large len -- read directly into user buffer */
        else if (state->how == COPY) {      /* read directly */
            if (gz_load(state, (unsigned char *)buf, n, &n) == -1)
                return 0;
        }

        /* large len -- decompress directly into user buffer */
        else {  /* state->how == GZIP */
            strm->avail_out = n;
            strm->next_out = (unsigned char *)buf;
            if (gz_decomp(state) == -1)
                return 0;
            n = state->x.have;
            state->x.have = 0;
        }

        /* update progress */
        len -= n;
        buf = (char *)buf + n;
        got += n;
        state->x.pos += n;
    } while (len);

    /* return number of bytes read into user buffer */
    return got;
}

/* -- see zlib.h -- */
int ZEXPORT gzread(gzFile file, voidp buf, unsigned len) {
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_statep)file;

    /* check that we're reading and that there's no (serious) error */
    if (state->mode != GZ_READ ||
            (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return -1;

    /* since an int is returned, make sure len fits in one, otherwise return
       with an error (this avoids a flaw in the interface) */
    if ((int)len < 0) {
        gz_error(state, Z_STREAM_ERROR, "request does not fit in an int");
        return -1;
    }

    /* read len or fewer bytes to buf */
    len = (unsigned)gz_read(state, buf, len);

    /* check for an error */
    if (len == 0 && state->err != Z_OK && state->err != Z_BUF_ERROR)
        return -1;

    /* return the number of bytes read (this is assured to fit in an int) */
    return (int)len;
}

/* -- see zlib.h -- */
z_size_t ZEXPORT gzfread(voidp buf, z_size_t size, z_size_t nitems,
                         gzFile file) {
    z_size_t len;
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return 0;
    state = (gz_statep)file;

    /* check that we're reading and that there's no (serious) error */
    if (state->mode != GZ_READ ||
            (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return 0;

    /* compute bytes to read -- error on overflow */
    len = nitems * size;
    if (size && len / size != nitems) {
        gz_error(state, Z_STREAM_ERROR, "request does not fit in a size_t");
        return 0;
    }

    /* read len or fewer bytes to buf, return the number of full items read */
    return len ? gz_read(state, buf, len) / size : 0;
}

/* -- see zlib.h -- */
#ifdef Z_PREFIX_SET
#  undef z_gzgetc
#else
#  undef gzgetc
#endif
int ZEXPORT gzgetc(gzFile file) {
    unsigned char buf[1];
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_statep)file;

    /* check that we're reading and that there's no (serious) error */
    if (state->mode != GZ_READ ||
        (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return -1;

    /* try output buffer (no need to check for skip request) */
    if (state->x.have) {
        state->x.have--;
        state->x.pos++;
        return *(state->x.next)++;
    }

    /* nothing there -- try gz_read() */
    return gz_read(state, buf, 1) < 1 ? -1 : buf[0];
}

int ZEXPORT gزgetc_(gzFile file) {
    return gzgetc(file);
}

/* -- see zlib.h -- */
int ZEXPORT gzungetc(int c, gzFile file) {
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_statep)file;

    /* in case this was just opened, set up the input buffer */
    if (state->mode == GZ_READ && state->how == LOOK && state->x.have == 0)
        (void)gz_look(state);

    /* check that we're reading and that there's no (serious) error */
    if (state->mode != GZ_READ ||
        (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return -1;

    /* process a skip request */
    if (state->seek) {
        state->seek = 0;
        if (gz_skip(state, state->skip) == -1)
            return -1;
    }

    /* can't push EOF */
    if (c < 0)
        return -1;

    /* if output buffer empty, put byte at end (allows more pushing) */
    if (state->x.have == 0) {
        state->x.have = 1;
        state->x.next = state->out + (state->size << 1) - 1;
        state->x.next[0] = (unsigned char)c;
        state->x.pos--;
        state->past = 0;
        return c;
    }

    /* if no room, give up (must have already done a gzungetc()) */
    if (state->x.have == (state->size << 1)) {
        gz_error(state, Z_DATA_ERROR, "out of room to push characters");
        return -1;
    }

    /* slide output data if needed and insert byte before existing data */
    if (state->x.next == state->out) {
        unsigned char *src = state->out + state->x.have;
        unsigned char *dest = state->out + (state->size << 1);
        while (src > state->out)
            *--dest = *--src;
        state->x.next = dest;
    }
    state->x.have++;
    state->x.next--;
    state->x.next[0] = (unsigned char)c;
    state->x.pos--;
    state->past = 0;
    return c;
}

/* -- see zlib.h -- */
char * ZEXPORT gzgets(gzFile file, char *buf, int len) {
    unsigned left, n;
    char *str;
    unsigned char *eol;
    gz_statep state;

    /* check parameters and get internal structure */
    if (file == NULL || buf == NULL || len < 1)
        return NULL;
    state = (gz_statep)file;

    /* check that we're reading and that there's no (serious) error */
    if (state->mode != GZ_READ ||
        (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return NULL;

    /* process a skip request */
    if (state->seek) {
        state->seek = 0;
        if (gz_skip(state, state->skip) == -1)
            return NULL;
    }

    /* copy output bytes up to new line or len - 1, whichever comes first --
       append a terminating zero to the string (we don't check for a zero in
       the contents, let the user worry about that) */
    str = buf;
    left = (unsigned)len - 1;
    if (left) do {
        /* assure that something is in the output buffer */
        if (state->x.have == 0 && gz_fetch(state) == -1)
            return NULL;                /* error */
        if (state->x.have == 0) {       /* end of file */
            state->past = 1;            /* read past end */
            break;                      /* return what we have */
        }

        /* look for end-of-line in current output buffer */
        n = state->x.have > left ? left : state->x.have;
        eol = (unsigned char *)memchr(state->x.next, '\n', n);
        if (eol != NULL)
            n = (unsigned)(eol - state->x.next) + 1;

        /* copy through end-of-line, or remainder if not found */
        memcpy(buf, state->x.next, n);
        state->x.have -= n;
        state->x.next += n;
        state->x.pos += n;
        left -= n;
        buf += n;
    } while (left && eol == NULL);

    /* return terminated string, or if nothing, end of file */
    if (buf == str)
        return NULL;
    buf[0] = 0;
    return str;
}

/* -- see zlib.h -- */
int ZEXPORT gzdirect(gzFile file) {
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return 0;
    state = (gz_statep)file;

    /* if the state is not known, but we can find out, then do so (this is
       mainly for right after a gzopen() or gzdopen()) */
    if (state->mode == GZ_READ && state->how == LOOK && state->x.have == 0)
        (void)gz_look(state);

    /* return 1 if transparent, 0 if processing a gzip stream */
    return state->direct;
}

/* -- see zlib.h -- */
int ZEXPORT gzclose_r(gzFile file) {
    int ret, err;
    gz_statep state;

    /* get internal structure */
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;

    /* check that we're reading */
    if (state->mode != GZ_READ)
        return Z_STREAM_ERROR;

    /* free memory and close file */
    if (state->size) {
        inflateEnd(&(state->strm));
        free(state->out);
        free(state->in);
    }
    err = state->err == Z_BUF_ERROR ? Z_BUF_ERROR : Z_OK;
    gz_error(state, Z_OK, NULL);
    free(state->path);
    ret = close(state->fd);
    free(state);
    return ret ? Z_ERRNO : err;
}
/* gzwrite.c -- zlib functions for writing gzip files
 * Copyright (C) 2004-2017 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

/* Initialize state for writing a gzip file.  Mark initialization by setting
   state->size to non-zero.  Return -1 on failure or 0 on success. */
local int gz_init(gz_statep state) {
    int ret;
    z_streamp strm = &(state->strm);

    /* allocate input and output buffers */
    state->in = (unsigned char *)malloc(state->want);
    state->out = (unsigned char *)malloc(state->want);
    if (state->in == NULL || state->out == NULL) {
        free(state->out);
        free(state->in);
        gz_error(state, Z_MEM_ERROR, "out of memory");
        return -1;
    }

    /* allocate deflate memory, set up for gzip compression */
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    ret = deflateInit2(strm, state->level, Z_DEFLATED,
                       15 + 16, 8, state->strategy);
    if (ret != Z_OK) {
        free(state->in);
        gz_error(state, Z_MEM_ERROR, "out of memory");
        return -1;
    }

    /* mark state as initialized */
    state->size = state->want;
    return 0;
}

/* Compress whatever is at the start of the input buffer until the input
   buffer is empty or the output buffer is full.  Return 1 if the input buffer
   is empty, 0 if the output buffer is full, or -1 on error. */
local int gz_comp(gz_statep state, int flush) {
    int ret;
    z_streamp strm = &(state->strm);

    /* allocate memory if this is the first time through */
    if (state->size == 0 && gz_init(state) == -1)
        return -1;

    /* run deflate() on provided input until output buffer is full, input
       buffer is empty, or end of data is flagged */
    for (;;) {
        /* if input buffer is empty and flush is Z_NO_FLUSH, return ok */
        if (strm->avail_in == 0 && flush == Z_NO_FLUSH)
            return 1;

        /* compress as much as provided input allows */
        ret = deflate(strm, flush);
        if (ret == Z_STREAM_ERROR) {
            gz_error(state, Z_STREAM_ERROR, "internal error: deflate stream corrupt");
            return -1;
        }

        /* check if output buffer is full */
        if (strm->avail_out == 0) {
            /* flush output buffer and check for write errors */
            if (state->str.avail_out && gz_compbuf(state) == -1)
                return -1;
            strm->avail_out = state->size;
            strm->next_out = state->out;
        }

        /* if deflate() returned Z_STREAM_END, clean up and return success */
        if (ret == Z_STREAM_END)
            return 1;

        /* check if deflate() consumed all input */
        if (strm->avail_in == 0) {
            /* reset input buffer for next data */
            strm->avail_in = state->size;
            strm->next_in = state->in;
            return 1;
        }
    }
}

/* Compress len bytes to the given file from buf.  Return the number of bytes
   written, or -1 on error. */
local int gz_write(gzFile file, voidpc buf, unsigned len) {
    unsigned put = len;
    gz_statep state = (gz_statep)file;
    z_streamp strm = &(state->strm);

    /* check for potential buffer overflow with len */
    if (len > UINT_MAX - strm->avail_in) {
        gz_error(state, Z_BUF_ERROR, "requested write length too large");
        return -1;
    }

    /* since an int is returned, make sure len fits in one, otherwise return
       with an error (this avoids a flaw in the interface) */
    if ((int)len < 0) {
        gz_error(state, Z_DATA_ERROR, "requested write length is negative");
        return -1;
    }

    /* ensure buffer is initialized */
    if (state->size == 0 && gz_init(state) == -1)
        return -1;

    /* check if we're in a valid state */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return -1;

    /* handle empty write */
    if (len == 0)
        return 0;

    /* compress len bytes from buf, writing to file and handling errors */
    strm->avail_in = len;
    strm->next_in = (voidpc)buf;
    if (gz_comp(state, Z_NO_FLUSH) != 1) {
        gz_error(state, Z_ERRNO, "compression error");
        return -1;
    }

    /* return number of bytes written (should equal len unless error) */
    return put - strm->avail_in;
}

/* Write buf to file, return number of bytes written (may be less than len) */
local int gz_compress(gz_statep state, voidpc buf, unsigned len) {
    unsigned n;
    z_streamp strm = &(state->strm);

    if (strm->avail_in != 0) {
        /* copy remaining input data into buffer */
        memmove(strm->next_in, state->in, strm->avail_in);
        strm->next_in = state->in;
    }

    /* write to buffer, compress when full */
    if (len < state->size) {
        n = len;
        memcpy(strm->next_in + strm->avail_in, buf, n);
        strm->avail_in += n;
        state->x.pos += n;
        return (int)n;
    } else {
        /* compress directly from user buffer */
        n = deflate(strm, Z_FULL_FLUSH);
        if (n != Z_OK) {
            gz_error(state, Z_ERRNO, "compression error");
            return -1;
        }
        state->x.pos += len;
        return len;
    }
}

/* Clean up compression state after error or completion */
local void gz_comp_cleanup(gz_statep state) {
    if (state->size) {
        deflateEnd(&state->strm);
        free(state->out);
        free(state->in);
        state->size = 0;
    }
}
/* gzlib.c -- zlib functions common to reading and writing gzip files
 * Copyright (C) 2004-2017 Mark Adler
 * For conditions of distribution and use, see http://www.zlib.net/zlib_license.html
 */

#include "gzguts.h"

#if defined(_WIN32) && !defined(__BORLANDC__)
#  define LSEEK _lseeki64
#else
#  define LSEEK lseek
#endif

/* Local functions */
local int gzcomp(gz_statep state, int flush);

/* ===========================================================================
     Opens a gzip (.gz) file for reading or writing. The mode parameter is as
   in fopen ("rb" or "wb") but can also include a compression level ("wb9") or
   a strategy: 'f' for filtered data as in "wb6f", 'h' for Huffman-only
   compression as in "wb1h", 'R' for run-length encoding as in "wb1R", or 'F'
   for fixed code compression as in "wb9F". 'T' enables transparent (uncompressed)
   writing. (See the description of deflateInit2 for more information about
   the strategy parameter.)

     gzopen can be used to read a file which is not in gzip format; in this
   case, gzread will directly read from the file without decompression.

     gzopen returns NULL if the file could not be opened or if there was
   insufficient memory to allocate the (de)compression state; errno can be
   checked to distinguish the two cases (if errno is zero, the zlib error is
   Z_MEM_ERROR).
*/
gzFile ZEXPORT gzopen(const char *path, const char *mode) {
    gz_statep state;
    size_t len;
    int oflag;
#ifdef O_CLOEXEC
    int cloexec = 0;
#endif
#ifdef O_EXCL
    int exclusive = 0;
#endif

    /* check input */
    if (path == NULL || mode == NULL)
        return NULL;

    /* allocate gz_state */
    state = (gz_statep)malloc(sizeof(gz_state));
    if (state == NULL)
        return NULL;
    memset(state, 0, sizeof(gz_state));
    state->level = Z_DEFAULT_COMPRESSION;
    state->strategy = Z_DEFAULT_STRATEGY;
    state->mode = GZ_NONE;

    /* save the path name for error messages */
    len = strlen(path);
    state->path = (char *)malloc(len + 1);
    if (state->path == NULL) {
        free(state);
        return NULL;
    }
    strcpy(state->path, path);

    /* compute the flags for open() */
    oflag = O_BINARY;
    switch (*mode++) {
    case 'r':
        state->mode = GZ_READ;
        oflag |= O_RDONLY;
        break;
    case 'w':
        state->mode = GZ_WRITE;
        oflag |= O_WRONLY | O_CREAT | O_TRUNC;
        break;
    case 'a':
        state->mode = GZ_WRITE;
        oflag |= O_WRONLY | O_CREAT | O_APPEND;
        break;
    default:
        free(state->path);
        free(state);
        return NULL;
    }

    while (*mode) {
        if (*mode >= '0' && *mode <= '9') {
            state->level = *mode - '0';
        } else {
            switch (*mode) {
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
            default:
                break;
            }
        }
        mode++;
    }

#ifdef O_CLOEXEC
    if (cloexec)
        oflag |= O_CLOEXEC;
#endif
#ifdef O_EXCL
    if (exclusive)
        oflag |= O_EXCL;
#endif

    /* open the file with the appropriate flags */
    state->fd = open(path, oflag, 0666);
    if (state->fd == -1) {
        free(state->path);
        free(state);
        return NULL;
    }

    /* For write modes, initialize compression */
    if (state->mode == GZ_WRITE) {
        state->out = (unsigned char *)malloc(GZBUFSIZE);
        if (state->out == NULL) {
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
        state->strm.zalloc = Z_NULL;
        state->strm.zfree = Z_NULL;
        state->strm.opaque = Z_NULL;

        if (state->direct) {  /* Direct (uncompressed) writing */
            state->strm.next_out = state->out;
            state->strm.avail_out = GZBUFSIZE;
        } else {
            int ret = deflateInit2(&state->strm, state->level, Z_DEFLATED,
                                  MAX_WBITS + 16, DEF_MEM_LEVEL, state->strategy);
            if (ret != Z_OK) {
                free(state->out);
                close(state->fd);
                free(state->path);
                free(state);
                return NULL;
            }
        }
    } else {  /* Read mode */
        state->in = (unsigned char *)malloc(GZBUFSIZE);
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

        if (inflateInit2(&state->strm, 47) != Z_OK) {  /* 47 for automatic header detection */
            free(state->in);
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
    }

    state->size = GZBUFSIZE;
    return (gzFile)state;
}

/* ===========================================================================
     Set the internal buffer size used by this library's functions. The
   default buffer size is 8192 bytes. This must be called after gzopen() or
   gzdopen(), and before any other calls that read or write the file. The
   buffer memory allocation is always deferred to this function. By default,
   the buffer is allocated when the first read or write is requested.
*/
int ZEXPORT gzbuffer(gzFile file, unsigned size) {
    gz_statep state = (gz_statep)file;
    if (state == NULL || state->mode == GZ_NONE || state->size != GZBUFSIZE)
        return -1;
    if (size < 2)
        size = 2;
    state->want = size;
    return 0;
}

/* ===========================================================================
     Flushes all pending output if necessary, closes the compressed file
   and deallocates all the (de)compression state. The return value is the zlib
   error number (see deflate.c and inflate.c).
*/
int ZEXPORT gzclose(gzFile file) {
    int ret = Z_OK;
    gz_statep state = (gz_statep)file;

    if (state == NULL)
        return Z_STREAM_ERROR;

    /* Handle write mode: flush and close */
    if (state->mode == GZ_WRITE) {
        if (!state->direct && gzcomp(state, Z_FINISH) == -1)
            ret = state->err;

        if (state->out != NULL) {
            free(state->out);
            state->out = NULL;
        }
        if (!state->direct)
            deflateEnd(&state->strm);
    } else if (state->mode == GZ_READ) {
        inflateEnd(&state->strm);
        if (state->in != NULL) {
            free(state->in);
            state->in = NULL;
        }
    }

    close(state->fd);
    free(state->path);
    free(state);
    return ret;
}

/* ===========================================================================
     Compress pending input data and write to output file. Returns the number
   of bytes written, or -1 on error.
*/
local int gzcomp(gz_statep state, int flush) {
    int ret, have;
    unsigned char *out = state->out;

    if (state->direct) {
        /* Direct write: just write the data */
        have = state->strm.avail_out;
        if (write(state->fd, out, have) != (ssize_t)have) {
            gz_error(state, Z_ERRNO, zstrerror());
            return -1;
        }
        state->strm.avail_out = GZBUFSIZE;
        state->strm.next_out = out;
        return (int)have;
    }

    /* Normal compressed write */
    do {
        ret = deflate(&state->strm, flush);
        if (ret == Z_STREAM_ERROR) {
            gz_error(state, Z_STREAM_ERROR, "deflate error");
            return -1;
        }
        have = GZBUFSIZE - state->strm.avail_out;
        if (have && write(state->fd, out, have) != (ssize_t)have) {
            gz_error(state, Z_ERRNO, zstrerror());
            return -1;
        }
        state->x.pos += have;
        state->strm.next_out = out;
        state->strm.avail_out = GZBUFSIZE;
    } while (state->strm.avail_in != 0 || (flush == Z_FINISH && ret != Z_STREAM_END));

    return 0;
}

/* ===========================================================================
     Update the compression level and strategy. This is only allowable when
   writing, and must be called immediately after gzopen(), before any data has
   been written.
*/
int ZEXPORT gzsetparams(gzFile file, int level, int strategy) {
    gz_statep state = (gz_statep)file;
    if (state == NULL || state->mode != GZ_WRITE || state->direct)
        return Z_STREAM_ERROR;

    /* Flush pending data */
    if (state->strm.avail_in && gzcomp(state, Z_BLOCK) == -1)
        return state->err;

    /* Validate parameters */
    if (level < Z_DEFAULT_COMPRESSION || level > Z_BEST_COMPRESSION ||
        strategy < Z_DEFAULT_STRATEGY || strategy > Z_FIXED)
        return Z_STREAM_ERROR;

    /* Update parameters */ 
    state->level = level;
    state->strategy = strategy;
    return deflateParams(&state->strm, level, strategy);
}