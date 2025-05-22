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

    /* Check for valid state structure and open mode */
    if (state->mode != GZ_READ && state->mode != GZ_WRITE)
        return Z_STREAM_ERROR;

    /* Flush and close the file if writing */
    if (state->mode == GZ_WRITE) {
        if (gz_comp(state, Z_FINISH) == -1)
            ret = state->err;
        if (gz_close(state->fd, state->path) != 0)
            ret = Z_ERRNO;
    }

    /* Free memory and reset state */
    if (state->size) {
        free(state->out);
        free(state->in);
    }
    free(state->path);
    free(state);

    return ret;
}