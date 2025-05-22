/* infcover.c -- test zlib's inflate routines
 * Copyright (C) 2002-2017 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

/* Local functions */
static void cover_support(void);
static void cover_test(void);
static int cover_back(void *desc, unsigned char *buf, unsigned len);
static void cover_inflate(void *desc, z_streamp strm, int flush);

/* Test inflate() with large buffers */
int main(void)
{
    fprintf(stderr, "Testing inflate routines...\n");
    cover_support();
    cover_test();
    return 0;
}

/* Test inflateBack() */
static void cover_support(void)
{
    int ret;
    z_stream strm;
    unsigned char win[32768];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = inflateBackInit(&strm, 15, win);
    if (ret != Z_OK) {
        fprintf(stderr, "inflateBackInit error: %d\n", ret);
        exit(1);
    }
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateBack(&strm, Z_NO_FLUSH, cover_back, Z_NULL, cover_inflate, Z_NULL);
    if (ret != Z_STREAM_END)
        fprintf(stderr, "inflateBack error: %d\n", ret);
    inflateBackEnd(&strm);
}

/* Test inflate() with full flush */
static void cover_test(void)
{
    int ret;
    z_stream strm;
    unsigned char out[4];

    /* Set up input and output buffers */
    static unsigned char in[] = {0x63, 0x00, 0x00, 0x00, 0x00, 0x00};
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        fprintf(stderr, "inflateInit error: %d\n", ret);
        exit(1);
    }
    strm.avail_in = sizeof(in);
    strm.next_in = in;
    strm.avail_out = sizeof(out);
    strm.next_out = out;
    do {
        ret = inflate(&strm, Z_SYNC_FLUSH);
    } while (ret == Z_OK);
    inflateEnd(&strm);
}

/* Callback functions for inflateBack() */
static int cover_back(void *desc, unsigned char *buf, unsigned len)
{
    (void)desc;
    (void)buf;
    return (int)len;  /* Force input buffer exhaustion */
}

static void cover_inflate(void *desc, z_streamp strm, int flush)
{
    (void)desc;
    (void)strm;
    (void)flush;
}
/* infcover.c_chunk2 - test zlib's inflate functions */
/* Copyright (C) 2023, 2024 The Zlib maintainers. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

#define local static

local void cover_support(void);
local void cover_test(void);

/* Test inflate() with large buffers. */
local int inf_large(z_streamp strm, int flush) {
    int ret;
    unsigned char input[4];
    unsigned char output[4];
    /* Fill input buffer with dummy data */
    memset(input, 0, sizeof(input));
    do {
        strm->avail_in = sizeof(input);
        strm->next_in = input;
        strm->avail_out = sizeof(output);
        strm->next_out = output;
        ret = inflate(strm, flush);
    } while (ret == Z_OK);
    return ret;
}

/* Test inflate() with small buffers. */
local int inf(z_streamp strm, int flush) {
    int ret;
    unsigned char input[4];
    unsigned char output[1];
    /* Fill input buffer with dummy data */
    memset(input, 0, sizeof(input));
    strm->avail_in = sizeof(input) - 1;
    strm->next_in = input;
    strm->avail_out = sizeof(output);
    strm->next_out = output;
    ret = inflate(strm, flush);
    if (ret == Z_BUF_ERROR)
        return Z_STREAM_END;
    return ret;
}

/* Test inflate() with wrapped buffers. */
local int cover_wrap(z_streamp strm) {
    return inflateReset(strm);
}

local void cover_test(void) {
    int n;
    z_stream strm;
    int (*inf_func)(z_streamp, int) = inflate;

    /* Test inflate with small and large buffers. */
    for (n = 0; n < 2; n++) {
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        if (n == 0)
            inf_func = inf;
        else
            inf_func = inf_large;
        if (inflateInit2(&strm, -15) != Z_OK)
            fprintf(stderr, "inflateInit2 error\n");
        if (inf_func(&strm, Z_FINISH) != Z_STREAM_END)
            fprintf(stderr, "inflate error\n");
        if (inflateEnd(&strm) != Z_OK)
            fprintf(stderr, "inflateEnd error\n");
    }

    /* Test cover_wrap function. */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    if (inflateInit2(&strm, -15) != Z_OK)
        fprintf(stderr, "inflateInit2 error\n");
    if (cover_wrap(&strm) != Z_OK)
        fprintf(stderr, "cover_wrap error\n");
    if (inflate(&strm, Z_FINISH) != Z_STREAM_END)
        fprintf(stderr, "inflate error\n");
    if (inflateEnd(&strm) != Z_OK)
        fprintf(stderr, "inflateEnd error\n");
}

local void cover_support(void) {
    fprintf(stdout, "Cover support check:\n");
    fprintf(stdout, "Inflate: %s\n", zlibVersion());
}

int main(void) {
    cover_test();
    cover_support();
    return 0;
}
    try("1f 8b 8 0 0 0 0 0 0 0 3 0 0 0 0 1", "incorrect data check", -1);
    try("1f 8b 8 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 1",
        "incorrect length check", -1);
    try("5 c0 21 d 0 0 0 80 b0 fe 6d 2f 91 6c", "pull 17", 0);
    try("5 e0 81 91 24 cb b2 2c 49 e2 f 2e 8b 9a 47 56 9f fb fe ec d2 ff 1f",
        "long code", 0);
    try("ed c0 1 1 0 0 0 40 20 ff 57 1b 42 2c 4f", "length extra", 0);
    try("ed cf c1 b1 2c 47 10 c4 30 fa 6f 35 1d 1 82 59 3d fb be 2e 2a fc f c",
        "long distance and extra", 0);
    try("ed c0 81 0 0 0 0 80 a0 fd a9 17 a9 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
        "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 6", "window end", 0);
    inf("2 8 20 80 0 3 0", "inflate_fast TYPE return", 0, -15, 258,
        Z_STREAM_END);
    inf("63 18 5 40 c 0", "window wrap", 3, -8, 300, Z_OK);
}

/* cover remaining lines in inftrees.c */
local void cover_trees(void)
{
    int ret;
    unsigned bits;
    unsigned short lens[16], work[16];
    code *next, table[ENOUGH_DISTS];

    /* we need to call inflate_table() directly in order to manifest not-
       enough errors, since zlib insures that enough is always enough */
    for (bits = 0; bits < 15; bits++)
        lens[bits] = (unsigned short)(bits + 1);
    lens[15] = 15;
    next = table;
    bits = 15;
    ret = inflate_table(DISTS, lens, 16, &next, &bits, work);
                                                assert(ret == 1);
    next = table;
    bits = 1;
    ret = inflate_table(DISTS, lens, 16, &next, &bits, work);
                                                assert(ret == 1);
    fputs("inflate_table not enough errors\n", stderr);
}

/* cover remaining inffast.c decoding and window copying */
local void cover_fast(void)
{
    inf("e5 e0 81 ad 6d cb b2 2c c9 01 1e 59 63 ae 7d ee fb 4d fd b5 35 41 68"
        " ff 7f 0f 0 0 0", "fast length extra bits", 0, -8, 258, Z_DATA_ERROR);
    inf("25 fd 81 b5 6d 59 b6 6a 49 ea af 35 6 34 eb 8c b9 f6 b9 1e ef 67 49"
        " 50 fe ff ff 3f 0 0", "fast distance extra bits", 0, -8, 258,
        Z_DATA_ERROR);
    inf("3 7e 0 0 0 0 0", "fast invalid distance code", 0, -8, 258,
        Z_DATA_ERROR);
    inf("1b 7 0 0 0 0 0", "fast invalid literal/length code", 0, -8, 258,
        Z_DATA_ERROR);
    inf("d c7 1 ae eb 38 c 4 41 a0 87 72 de df fb 1f b8 36 b1 38 5d ff ff 0",
        "fast 2nd level codes and too far back", 0, -8, 258, Z_DATA_ERROR);
    inf("63 18 5 8c 10 8 0 0 0 0", "very common case", 0, -8, 259, Z_OK);
    inf("63 60 60 18 c9 0 8 18 18 18 26 c0 28 0 29 0 0 0",
        "contiguous and wrap around window", 6, -8, 259, Z_OK);
    inf("63 0 3 0 0 0 0 0", "copy direct from output", 0, -8, 259,
        Z_STREAM_END);
}

int main(void)
{
    fprintf(stderr, "%s\n", zlibVersion());
    cover_support();
    cover_wrap();
    cover_back();
    cover_inflate();
    cover_trees();
    cover_fast();
    return 0;
}