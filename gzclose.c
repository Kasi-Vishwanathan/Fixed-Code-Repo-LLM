/* gzclose.c -- zlib gzclose() function
 * Copyright (C) 2004, 2010 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

int ZEXPORT gzclose(gzFile file) {
    gz_statep state;
    int ret = Z_OK;

    if (file == NULL)
        return Z_STREAM_ERROR;

    state = (gz_statep)file;

    if (state->mode != GZ_READ && state->mode != GZ_WRITE)
        return Z_STREAM_ERROR;

#ifndef NO_GZCOMPRESS
    if (state->mode == GZ_WRITE) {
        int flush_ret;
        if (state->size) {
            /* Flush remaining data with Z_FINISH */
            flush_ret = deflate(&state->strm, Z_FINISH);
            if (flush_ret != Z_STREAM_END && flush_ret != Z_OK) {
                ret = (flush_ret == Z_STREAM_ERROR) ? Z_STREAM_ERROR : Z_BUF_ERROR;
            } else {
                /* Write any remaining output */
                if (state->strm.avail_out != state->size) {
                    if (gz_comp(state, Z_FINISH) == -1)
                        ret = state->err;
                }
            }
        }
        /* Clean up compression resources */
        deflateEnd(&state->strm);
        /* Free buffers and path */
        if (state->size) {
            free(state->out);
            free(state->in);
        }
        free(state->path);
        /* Close the file descriptor */
        if (close(state->fd) == -1)
            ret = Z_ERRNO;
        free(state);
    } else
#endif
    {  /* Mode is GZ_READ */
        /* Clean up decompression resources */
        inflateEnd(&state->strm);
        /* Free buffers and path */
        free(state->out);
        free(state->in);
        free(state->path);
        /* Close the file descriptor */
        if (close(state->fd) == -1)
            ret = Z_ERRNO;
        free(state);
    }

    return ret;
}