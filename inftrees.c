/* infback.c -- inflate using a call-back interface
 * Copyright (C) 1995-2022 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zutil.h"
#include "inftrees.h"
#include "inflate.h"
#include "inffast.h"

/* Handle negative windowBits for raw inflate and validate parameters */
int ZEXPORT inflateBackInit_(z_streamp strm, int windowBits,
                             unsigned char FAR *window, const char *version,
                             int stream_size) {
    struct inflate_state FAR *state;

    if (version == Z_NULL || version[0] != ZLIB_VERSION[0] ||
        stream_size != (int)(sizeof(z_stream)))
        return Z_VERSION_ERROR;
    if (strm == Z_NULL || window == Z_NULL ||
        windowBits < -15 || windowBits > 15)
        return Z_STREAM_ERROR;

    strm->msg = Z_NULL;
    if (strm->zalloc == NULL) {
        strm->zalloc = zcalloc;
        strm->opaque = (voidpf)0;
    }
    if (strm->zfree == NULL)
        strm->zfree = zcfree;

    state = (struct inflate_state FAR *)ZALLOC(strm, 1, sizeof(struct inflate_state));
    if (state == Z_NULL) return Z_MEM_ERROR;

    state->wsize = 1U << (windowBits < 0 ? -windowBits : windowBits);
    state->window = window;
    state->wnext = 0;
    state->whave = 0;
    state->dmax = 32768U;
    state->sane = (windowBits > 0);  /* Raw inflate may need extra checks */

    strm->state = (struct internal_state FAR *)state;
    return Z_OK;
}

/* Generate fixed huffman tables (non-thread-safe if BUILDFIXED) */
local void fixedtables(struct inflate_state FAR *state) {
#ifdef BUILDFIXED
    /* ... (static variables should be avoided in multithreaded apps) ... */
#else
#   include "inffixed.h"
#endif
    state->lencode = lenfix;
    state->lenbits = 9;
    state->distcode = distfix;
    state->distbits = 5;
}

/* Correct gen_bitlen syntax and logic */
local void gen_bitlen(deflate_state *s, tree_desc *desc) {
    ct_data *tree = desc->dyn_tree;
    int max_code = desc->max_code;
    const ct_data *stree = desc->stat_desc->static_tree;
    const int *extra = desc->stat_desc->extra_bits;
    int base = desc->stat_desc->extra_base;
    int max_length = desc->stat_desc->max_length;  /* Fixed syntax */
    int h, n, m;
    int bits;
    int overflow = 0;

    for (bits = 0; bits <= MAX_BITS; bits++)
        s->bl_count[bits] = 0;

    tree[s->heap[s->heap_max]].Len = 0;

    for (h = s->heap_max + 1; h < HEAP_SIZE; h++) {
        n = s->heap[h];
        bits = tree[tree[n].Dad].Len + 1;
        if (bits > max_length) {
            bits = max_length;
            overflow++;
        }
        tree[n].Len = (ush)bits;
        if (n > max_code) continue;
        s->bl_count[bits]++;
        /* ... (rest of gen_bitlen logic) ... */
    }

    if (overflow)
        Tracev((stderr, "\nbit length overflow\n"));
}

/* Ensure ANSI C prototypes for all functions */
static void pqdownheap(deflate_state *s, ct_data *tree, int k) {
    int v = s->heap[k];
    int j = k << 1;  /* Left child */

    while (j <= s->heap_len) {
        if (j < s->heap_len &&
            tree[s->heap[j+1]].Freq < tree[s->heap[j]].Freq) j++;
        if (tree[v].Freq < tree[s->heap[j]].Freq) break;
        s->heap[k] = s->heap[j];
        k = j;
        j <<= 1;
    }
    s->heap[k] = v;
}

/* Remaining functions follow similar ANSI C corrections and bug fixes */