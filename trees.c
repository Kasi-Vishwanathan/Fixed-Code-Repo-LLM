/* trees.c -- output deflated data using Huffman coding
 * Copyright (C) 1995-2024 Jean-loup Gailly
 * detect_data_type() function provided freely by Cosmin Truta, 2006
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/*
 *  ALGORITHM
 *
 *      The "deflation" process uses several Huffman trees. The more
 *      common source values are represented by shorter bit sequences.
 *
 *      Each code tree is stored in a compressed form which is itself
 * a Huffman encoding of the lengths of all the code strings (in
 * ascending order by source values).  The actual code strings are
 * reconstructed from the lengths in the inflate process, as described
 * in the deflate specification.
 *
 *  REFERENCES
 *
 *      Deutsch, L.P.,"'Deflate' Compressed Data Format Specification".
 *      Available in ftp.uu.net:/pub/archiving/zip/doc/deflate-1.1.doc
 *
 *      Storer, James A.
 *          Data Compression:  Methods and Theory, pp. 49-50.
 *          Computer Science Press, 1988.  ISBN 0-7167-8156-5.
 *
 *      Sedgewick, R.
 *          Algorithms, p290.
 *          Addison-Wesley, 1983. ISBN 0-201-06672-6.
 */

/* @(#) $Id$ */

/* #define GEN_TREES_H */

#include "deflate.h"

#ifdef ZLIB_DEBUG
#  include <ctype.h>
#endif

/* ===========================================================================
 * Constants
 */

#define MAX_BL_BITS 7
/* Bit length codes must not exceed MAX_BL_BITS bits */

#define END_BLOCK 256
/* end of block literal code */

#define REP_3_6      16
/* repeat previous bit length 3-6 times (2 bits of repeat count) */

#define REPZ_3_10    17
/* repeat a zero length 3-10 times  (3 bits of repeat count) */

#define REPZ_11_138  18
/* repeat a zero length 11-138 times  (7 bits of repeat count) */

local const int extra_lbits[LENGTH_CODES] /* extra bits for each length code */
   = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

local const int extra_dbits[D_CODES] /* extra bits for each distance code */
   = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

local const int extra_blbits[BL_CODES]/* extra bits for each bit length code */
   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};

local const uch bl_order[BL_CODES]
   = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
/* The lengths of the bit length codes are sent in order of decreasing
 * probability, to avoid transmitting the lengths for unused bit length codes.
 */

/* ===========================================================================
 * Local data. These are initialized only once.
 */

#define DIST_CODE_LEN  512 /* see definition of array dist_code below */

#if defined(GEN_TREES_H) || !defined(STDC)
/* non ANSI compilers may not accept trees.h */

local ct_data static_ltree[L_CODES+2];
/* The static literal tree. Since the bit lengths are imposed, there is no
 * need for the L_CODES extra codes used during heap construction. However
 * The codes 286 and 287 are needed to build a canonical tree (see _tr_init
 * below).
 */

local ct_data static_dtree[D_CODES];
/* The static distance tree. (Actually a trivial tree since all codes use
 * 5 bits.)
 */

uch _dist_code[DIST_CODE_LEN];
/* Distance codes. The first 256 values correspond to the distances
 * 3 .. 258, the last 256 values correspond to the top 8 bits of
 * the 15 bit distances.
 */

uch _length_code[MAX_MATCH-MIN_MATCH+1];
/* length code for each normalized match length (0 == MIN_MATCH) */

local int base_length[LENGTH_CODES];
/* First normalized length for each code (0 = MIN_MATCH) */

local int base_dist[D_CODES];
/* First normalized distance for each code (0 = distance of 1) */

#else
#  include "trees.h"
#endif /* GEN_TREES_H */

struct static_tree_desc_s {
    const ct_data *static_tree;  /* static tree or NULL */
    const intf *extra_bits;      /* extra bits for each code or NULL */
    int     extra_base;          /* base index for extra_bits */
    int     elems;               /* max number of elements in the tree */
    int     max_length;          /* max bit length for the codes */
};

#ifdef NO_INIT_GLOBAL_POINTERS
#  define TCONST
#else
#  define TCONST const
#endif

local TCONST static_tree_desc static_l_desc =
{static_ltree, extra_lbits, LITERALS+1, L_CODES, MAX_BITS};

local TCONST static_tree_desc static_d_desc =
{static_dtree, extra_dbits, 0,          D_CODES, MAX_BITS};

local TCONST static_tree_desc static_bl_desc =
{(const ct_data *)0, extra_blbits, 0,   BL_CODES, MAX_BL_BITS};

/* ===========================================================================
 * Output a short LSB first on the stream.
 * IN assertion: there is enough room in pendingBuf.
 */
#define put_short(s, w) { \
    put_byte(s, (uch)((w) & 0xff)); \
    put_byte(s, (uch)((ush)(w) >> 8)); \
}

/* ===========================================================================
 * Reverse the first len bits of a code, using straightforward code (a faster
 * method would use a table)
 * IN assertion: 1 <= len <= 15
 */
local unsigned bi_reverse(unsigned code, int len) {
    register unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

/* ===========================================================================
 * Flush the bit buffer, keeping at most 7 bits in it.
 */
local void bi_flush(deflate_state *s) {
    if (s->bi_valid == 16) {
        put_short(s, s->bi_buf);
        s->bi_buf = 0;
        s->bi_valid = 0;
    } else if (s->bi_valid >= 8) {
        put_byte(s, (Byte)s->bi_buf);
        s->bi_buf >>= 8;
        s->bi_valid -= 8;
    }
}

/* ===========================================================================
 * Flush the bit buffer and align the output on a byte boundary
 */
local void bi_windup(deflate_state *s) {
    if (s->bi_valid > 8) {
        put_short(s, s->bi_buf);
    } else if (s->bi_valid > 0) {
        put_byte(s, (Byte)s->bi_buf);
    }
    s->bi_used = ((s->bi_valid - 1) & 7) + 1;
    s->bi_buf = 0;
    s->bi_valid = 0;
#ifdef ZLIB_DEBUG
    s->bits_sent = (s->bits_sent + 7) & ~7;
#endif
}

/* ===========================================================================
 * Generate the codes for a given tree and bit counts (which need not be
 * optimal).
 * IN assertion: the array bl_count contains the bit length statistics for
 * the given tree and the field len is set for all tree elements.
 * OUT assertion: the field code is set for all tree elements of non
 *     zero code length.
 */
local void gen_codes(ct_data *tree, int max_code, ushf *bl_count) {
    ush next_code[MAX_BITS+1]; /* next code value for each bit length */
    unsigned code = 0;         /* running code value */
    int bits;                  /* bit index */
    int n;                     /* code index */

    /* The distribution counts are first used to generate the code values
     * without bit reversal.
     */
    for (bits = 1; bits <= MAX_BITS; bits++) {
        code = (code + bl_count[bits - 1]) << 1;
        next_code[bits] = (ush)code;
    }
    /* Check that the bit counts in bl_count are consistent. The last code
     * must be all ones.
     */
    Assert (code + bl_count[MAX_BITS] - 1 == (1 << MAX_BITS) - 1,
            "inconsistent bit counts");
    Tracev((stderr,"\ngen_codes: max_code %d ", max_code));

    for (n = 0;  n <= max_code; n++) {
        int len = tree[n].Len;
        if (len == 0) continue;
        /* Now reverse the bits */
        tree[n].Code = (ush)bi_reverse(next_code[len]++, len);

        Tracecv(tree != static_ltree, (stderr,"\nn %3d %c l %2d c %4x (%x) ",
            n, (isgraph(n) ? n : ' '), len, tree[n].Code, next_code[len] - 1));
    }
}

#ifdef GEN_TREES_H
local void gen_trees_header(void);
#endif

#ifndef ZLIB_DEBUG
#  define send_code(s, c, tree) send_bits(s, tree[c].Code, tree[c].Len)
   /* Send a code of the given tree. c and tree must not have side effects */

#else /* !ZLIB_DEBUG */
#  define send_code(s, c, tree) \
     { if (z_verbose>2) fprintf(stderr,"\ncd %3d ",(c)); \
       send_bits(s, tree[c].Code, tree[c].Len); }
#endif

/* ===========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
#ifdef ZLIB_DEBUG
local void send_bits(deflate_state *s, int value, int length) {
    Tracevv((stderr," l %2d v %4x ", length, value));
    Assert(length > 0 && length <= 15, "invalid length");
    s->bits_sent += (ulg)length;

    /* If not enough room in bi_buf, use (valid) bits from bi_buf and
     * (16 - bi_valid) bits from value, leaving (width - (16 - bi_valid))
     * unused bits in value.
     */
    if (s->bi_valid > (int)Buf_size - length) {
        s->bi_buf |= (ush)value << s->bi_valid;
        put_short(s, s->bi_buf);
        s->bi_buf = (ush)value >> (Buf_size - s->bi_valid);
        s->bi_valid += length - Buf_size;
    } else {
        s->bi_buf |= (ush)value << s->bi_valid;
        s->bi_valid += length;
    }
}
#else /* !ZLIB_DEBUG */

#define send_bits(s, value, length) \
{ int len = length;\
  if (s->bi_valid > (int)Buf_size - len) {\
    int val = (int)value;\
    s->bi_buf |= (ush)val << s->bi_valid;\
    put_short(s, s->bi_buf);\
    s->bi_buf = (ush)val >> (Buf_size - s->bi_valid);\
    s->bi_valid += len - Buf_size;\
  } else {\
    s->bi_buf |= (ush)(value) << s->bi_valid;\
    s->bi_valid += len;\
  }\
}
#endif /* ZLIB_DEBUG */


/* the arguments must not have side effects */

/* ===========================================================================
 * Initialize the various 'constant' tables.
 */
local void tr_static_init(void) {
#if defined(GEN_TREES_H) || !defined(STDC)
    static int static_init_done = 0;
    int n;        /* iterates over tree elements */
    int bits;     /* bit counter */
    int length;   /* length value */

/* tree.c - Output generated by gen_trees_header for Huffman tree data */

#include <stdio.h>

/* Declare external variables from other parts of zlib */
extern const char static_ltree[];
extern const char static_dtree[];
extern const int extra_lbits[];
extern const int extra_dbits[];
extern const int extra_lbits[];
extern const int extra_dbits[];
extern const int base_length[];
extern const int base_dist[];

/* Local variables */
static int static_l_desc = 0, static_d_desc = 0, static_bl_desc = 0;

void gen_trees_header(void) {
    FILE *header = fopen("trees.h", "w");
    if (!header) {
        perror("Failed to open trees.h");
        return;
    }

    fprintf(header, "/* header generated automatically by gen_trees_header */\n\n");
    fprintf(header, "#define MAX_BL_BITS %d\n", static_l_desc);
    fprintf(header, "static const static_ltree LENGTH_OFFSET = %d;\n", static_ltree[0]);
    fprintf(header, "static const static_dtree DISTANCE_OFFSET = %d;\n", static_dtree[0]);
    /* Additional generated code... */

    fclose(header);
}
/* trees.c */
/* @(#) $Id$ */

#include "deflate.h"

/* Forward declarations */
static void send_bits(deflate_state *s, int value, int length);

/* ===========================================================================
 * Send a literal or distance tree in compressed form, using the codes in
 * bl_tree.
 */
void _tr_send_bits(deflate_state *s, int value, int length) {
    send_bits(s, value, length);
}

/* ===========================================================================
 * Output a byte on the stream.
 * IN assertion: there is enough room in pending_buf.
 */
#define put_short(s, w) { \
    put_byte(s, (uch)((w) & 0xff)); \
    put_byte(s, (uch)((ush)(w) >> 8)); \
}

/* ===========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
static void send_bits(deflate_state *s, int value, int length) {
    if (s->bi_valid > (int)Buf_size - length) {
        s->bi_buf |= (USH)value << s->bi_valid;
        put_short(s, s->bi_buf);
        s->bi_buf = (USH)value >> (Buf_size - s->bi_valid);
        s->bi_valid += length - Buf_size;
    } else {
        s->bi_buf |= (USH)value << s->bi_valid;
        s->bi_valid += length;
    }
}

/* ===========================================================================
 * Flush the bit buffer and align the output on a byte boundary
 */
void _tr_align(deflate_state *s) {
    send_bits(s, STATIC_TREES<<1, 3);
    put_short(s, (USH)static_ltree[END_BLOCK].Code);
    put_short(s, (USH)static_ltree[END_BLOCK].Len);
    bi_flush(s);
}

/* ===========================================================================
 * Flush the bit buffer, keeping at least 7 bits in it.
 */
void _tr_flush_bits(deflate_state *s) {
    bi_flush(s);
}

/* ===========================================================================
 * Determine the best encoding for the current block: dynamic trees, static
 * trees or store, and write out the encoded block.
 */
void _tr_flush_block(deflate_state *s, charf *buf, ulg stored_len, int eof) {
    ulg max_block_size = 0xffff;
    ulg stored_len_plus = (ulg)((stored_len + 4) << 3);

    if (s->compressed_len + stored_len_plus > max_block_size) {
        stored_len = max_block_size - s->compressed_len - 5;
    }

    /* ... rest of the function logic remains unchanged ... */
}

/* ===========================================================================
 * Send the block data compressed using the given Huffman trees
 */
static void compress_block(deflate_state *s, const ct_data *ltree, const ct_data *dtree) {
    unsigned dist;
    int lc;

    if (s->last_lit != 0) do {
        /* ... existing logic ... */
    } while (--s->last_lit != 0);
}

/* ===========================================================================
 * Send the block data compressed using the given Huffman trees
 */
void _tr_compress_block(deflate_state *s, const ct_data *ltree, const ct_data *dtree) {
    compress_block(s, ltree, dtree);
}
/* tree.c */
#include <stddef.h>
#include "deflate.h"

/* ===========================================================================
 * Compress a block using the given Huffman trees.
 */
static void compress_block(deflate_state *s, const ct_data *ltree, const ct_data *dtree)
{
    unsigned dist;      /* distance of matched string */
    int lc;             /* match length or unmatched char (if dist == 0) */
    unsigned lx = 0;    /* running index in l_buf */
    unsigned code;      /* the code to send */
    int extra;          /* number of extra bits to send */

    if (s->last_lit != 0) do {
        dist = s->d_buf[lx];
        lc = s->l_buf[lx++];
        if (dist == 0) {
            send_code(lc, ltree); /* send a literal byte */
        } else {
            /* send length and distance */
            code = length_code[lc];
            send_code(code+LITERALS+1, ltree); /* send length code */
            extra = extra_lbits[code];
            if (extra != 0) {
                lc -= base_length[code];
                send_bits(s, lc, extra);
            }
            dist--; /* dist is now the match distance - 1 */
            code = d_code(dist);
            send_code(code, dtree); /* send distance code */
            extra = extra_dbits[code];
            if (extra != 0) {
                dist -= (unsigned)base_dist[code];
                send_bits(s, dist, extra);
            }
        }
    } while (lx < s->last_lit);
}

/* ===========================================================================
 * Restore the heap property by moving down the tree starting at node k,
 * exchanging a node with the smallest of its two sons if necessary, stopping
 * when the heap property is re-established.
 */
static void pqdownheap(deflate_state *s, ct_data *tree, int k)
{
    int v = s->heap[k];
    int j = k << 1;  /* left son of k */
    while (j <= s->heap_len) {
        if (j < s->heap_len &&
            smaller(tree, s->heap[j+1], s->heap[j], s->depth)) {
            j++;
        }
        if (smaller(tree, v, s->heap[j], s->depth)) break;
        s->heap[k] = s->heap[j];
        k = j;
        j <<= 1;
    }
    s->heap[k] = v;
}

/* ===========================================================================
 * Compute the optimal bit lengths for a tree and update the total bit length
 * for the current block.
 */
void gen_bitlen(deflate_state *s, tree_desc *desc)
{
    ct_data *tree = desc->dyn_tree;
    int max_code = desc->max_code;
    const ct_data *stree = desc->stat_desc->static_tree;
    const intf *extra = desc->stat_desc->extra_bits;
    int base = desc->stat_desc->extra_base;
    int max_length = desc->stat_desc->max_length;
    int h;              /* heap index */
    int n, m;           /* iterate over the tree elements */
    int bits;           /* bit length */
    int xbits;          /* extra bits */
    ush f;              /* frequency */
    int overflow = 0;   /* number of elements with bit length too large */

    for (bits = 0; bits <= MAX_BITS; bits++) s->bl_count[bits] = 0;

    /* First pass: compute the optimal bit lengths (which may overflow). */
    tree[s->heap[s->heap_max]].Len = 0; /* root of the heap */

    for (h = s->heap_max+1; h < HEAP_SIZE; h++) {
        n = s->heap[h];
        bits = tree[tree[n].Dad].Len + 1;
        if (bits > max_length) {
            bits = max_length;
            overflow++;
        }
        tree[n].Len = (ush)bits;
        if (n > max_code) continue; /* not a leaf node */

        s->bl_count[bits]++;
        xbits = 0;
        if (n >= base) xbits = extra[n - base];
        f = tree[n].Freq;
        s->opt_len += (ulg)f * (bits + xbits);
        if (stree) s->static_len += (ulg)f * (stree[n].Len + xbits);
    }
    /* ... (rest of function remains unchanged) ... */
}