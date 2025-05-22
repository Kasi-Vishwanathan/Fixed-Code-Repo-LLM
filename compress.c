/* compress.c */
#include "zlib.h"

int ZEXPORT uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen) {
    z_stream stream;
    int err;
    uLong len, left;
    Byte buf[1];  // Temporary buffer for detecting output when dest is NULL or destLen is 0

    // Handle cases where dest is NULL or destLen is 0 by using a temporary buffer
    if (dest == NULL || *destLen == 0) {
        left = 1;
        dest = buf;
    } else {
        left = *destLen;
    }

    // Initialize the z_stream structure
    stream.next_in = (z_const Bytef *)source;
    stream.avail_in = 0;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    err = inflateInit(&stream);
    if (err != Z_OK) {
        return err;
    }

    stream.next_out = dest;
    stream.avail_out = 0;

    len = sourceLen;

    // Process input and output in chunks to handle large sizes and partial processing
    do {
        // Set up the output buffer space
        if (stream.avail_out == 0) {
            stream.avail_out = (left > (uLong)UINT_MAX) ? UINT_MAX : (uInt)left;
            left -= stream.avail_out;
        }

        // Set up the input buffer
        if (stream.avail_in == 0) {
            stream.avail_in = (len > (uLong)UINT_MAX) ? UINT_MAX : (uInt)len;
            len -= stream.avail_in;
        }

        err = inflate(&stream, Z_NO_FLUSH);
    } while (err == Z_OK);

    // Adjust the source and destination lengths after processing
    sourceLen -= len + stream.avail_in;
    if (dest != buf) {
        *destLen = stream.total_out;
    } else {
        // Check if data was produced when using the temporary buffer
        if (stream.total_out > 0) {
            err = Z_BUF_ERROR;
        }
    }

    // Clean up and determine the final error code
    inflateEnd(&stream);

    if (err == Z_STREAM_END) {
        return Z_OK;
    } else if (err == Z_NEED_DICT) {
        return Z_DATA_ERROR;
    } else if (err == Z_BUF_ERROR && (left + stream.avail_out) != 0) {
        return Z_DATA_ERROR;
    } else {
        return err;
    }
}