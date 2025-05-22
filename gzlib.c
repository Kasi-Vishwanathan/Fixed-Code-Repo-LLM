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
local int gzcomp(gz_statep state);

/* ===========================================================================
     Opens a gzip (.gz) file for reading or writing. The mode parameter is as
   in fopen ("rb" or "wb") but can also include a compression level ("wb9") or
   a strategy: 'f' for filtered data as in "wb6f", 'h' for Huffman-only
   compression as in "wb1h", 'R' for run-length encoding as in "wb1R", or 'F'
   for fixed code compression as in "wb9F". (See the description of
   deflateInit2 for more information about the strategy parameter.)

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
    state->size = 0;            /* no buffers allocated yet */
    state->want = GZBUFSIZE;    /* requested buffer size */
    state->msg = NULL;          /* no error message yet */

    /* save the path name for error messages */
    state->path = (char *)malloc(strlen(path) + 1);
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
                /* ignore invalid options */
                break;
            }
        }
        mode++;
    }

    /* open the file with the appropriate flags (or just use fd) */
    state->fd = open(path, oflag, 0666);

    if (state->fd == -1) {
        free(state->path);
        free(state);
        return NULL;
    }

    /* initialize */
    if (state->mode == GZ_WRITE) {
        /* write buffer */
        state->out = (unsigned char *)malloc(state->want);
        if (state->out == NULL) {
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
        /* compression parameters */
        state->strm.zalloc = Z_NULL;
        state->strm.zfree = Z_NULL;
        state->strm.opaque = Z_NULL;
        if (deflateInit2(&(state->strm), state->level, Z_DEFLATED,
                         MAX_WBITS + 16, DEF_MEM_LEVEL, state->strategy) != Z_OK) {
            free(state->out);
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
    } else {
        /* read buffer */
        state->in = (unsigned char *)malloc(state->want);
        if (state->in == NULL) {
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
        /* compression parameters */
        state->strm.zalloc = Z_NULL;
        state->strm.zfree = Z_NULL;
        state->strm.opaque = Z_NULL;
        state->strm.avail_in = 0;
        if (inflateInit2(&(state->strm), 47) != Z_OK) {
            free(state->in);
            close(state->fd);
            free(state->path);
            free(state);
            return NULL;
        }
    }
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
    gz_statep state;

    /* check and set buffer size */
    if (file == NULL)
        return -1;
    state = (gz_statep)file;
    if (state->size != 0)
        return -1;
    if (size < 2)
        size = 2;
    state->want = size;
    return 0;
}

/* ===========================================================================
     Update the compression level and strategy. This is only allowed when
   writing, and must be called immediately after gzopen() or gzdopen(), before
   any write has occurred.
*/
int ZEXPORT gzsetparams(gzFile file, int level, int strategy) {
    gz_statep state;
    z_streamp strm;

    /* check input */
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;
    strm = &(state->strm);

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return Z_STREAM_ERROR;

    /* if no change is needed, do nothing */
    if (level == state->level && strategy == state->strategy)
        return Z_OK;

    /* check for valid parameters */
    if (level < Z_DEFAULT_COMPRESSION || level > Z_BEST_COMPRESSION ||
        strategy < Z_DEFAULT_STRATEGY || strategy > Z_FIXED)
        return Z_STREAM_ERROR;

    /* adjust compression parameters */
    if (state->size) {
        /* flush previous data with previous parameters */
        if (gz_comp(state, Z_PARTIAL_FLUSH) == -1)
            return state->err;
    }
    state->level = level;
    state->strategy = strategy;
    return deflateParams(strm, level, strategy);
}

/* ===========================================================================
     Flushes all pending output if necessary, closes the compressed file
   and deallocates all the (de)compression state. The return value is the zlib
   error number (see deflate.c and inflate.c).
*/
int ZEXPORT gzclose(gzFile file) {
    int ret = Z_OK;
    gz_statep state;

    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;

    if (state->mode == GZ_WRITE) {
        if (gz_comp(state, Z_FINISH) == -1)
            return state->err;
        if (state->out_pos > 0) {
            if (write(state->fd, state->out, state->out_pos) < 0) {
                free(state->out);
                free(state->path);
                free(state);
                return Z_ERRNO;
            }
        }
        deflateEnd(&(state->strm));
        free(state->out);
    } else if (state->mode == GZ_READ) {
        inflateEnd(&(state->strm));
        free(state->in);
    }
    close(state->fd);
    free(state->path);
    free(state);
    return ret;
}

/* ===========================================================================
     Compress whatever is at the start of the input buffer to the output buffer.
   Return the number of bytes written to the output buffer, or -1 if there was
   an error reading from the input file.
*/
local int gzcomp(gz_statep state) {
    /* placeholder for compression logic */
    return 0;
}
/* gzclose.c -- zlib gzclose() function
 * Copyright (C) 2004, 2010, 2012, 2016 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

int gzclose(gzFile file) {
    int ret = Z_OK;
    gz_statep state;

    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;

    /* Validate mode */
    if (state->mode != GZ_READ && state->mode != GZ_WRITE) {
        free(state);
        return Z_STREAM_ERROR;
    }

    /* Handle write mode: flush and close */
    if (state->mode == GZ_WRITE) {
        /* Flush pending data */
        if (state->size != 0 && gz_comp(state, Z_FINISH) == -1) {
            ret = state->err != Z_OK ? state->err : Z_BUF_ERROR;
        }
        /* Close the file descriptor */
        if (gz_close(state->fd, state->path) != 0)
            ret = Z_ERRNO;
    }

    /* Free allocated memory */
    if (state->size) {
        free(state->out);
        free(state->in);
    }
    free(state->path);
    free(state);

    return ret;
}