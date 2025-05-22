/* crc32.c_chunk1 */
#include "zlib.h"
#include <stdint.h>
#include <stddef.h>

#define CRCPOLY        0xedb88320UL  /* CRC-32 (Ethernet, ZIP) */
#define CRCPOLY_CRC32C 0x82f63b78UL  /* CRC-32C (iSCSI) */
#define CRCPOLY_BZIP2  0x04c11db7UL  /* CRC-32 (BZIP2) */

static uint32_t crc32_table[256];
static uint32_t crc32c_table[256];
static uint32_t crc32_bzip2_table[256];

/* Initialize CRC table for a given polynomial */
static void make_crc_table(uint32_t *table, uint32_t poly) {
    for (uint32_t n = 0; n < 256; n++) {
        uint32_t c = n;
        for (int k = 0; k < 8; k++) {
            c = (c & 1) ? (c >> 1) ^ poly : c >> 1;
        }
        table[n] = c;
    }
}

/* Generic CRC calculation using specified table */
static uint32_t crc32_using_table(uint32_t crc, const uint8_t *buf, size_t len, const uint32_t *table) {
    crc ^= 0xffffffffUL;
    while (len--) {
        crc = table[(crc ^ *buf++) & 0xff] ^ (crc >> 8);
    }
    return crc ^ 0xffffffffUL;
}

/* CRC-32 Main Function */
uint32_t ZEXPORT crc32(uint32_t crc, const uint8_t *buf, size_t len) {
    static int initialized = 0;
    if (!initialized) {
        make_crc_table(crc32_table, CRCPOLY);
        initialized = 1;
    }
    return crc32_using_table(crc, buf, len, crc32_table);
}

/* CRC-32C (iSCSI) */
uint32_t ZEXPORT crc32c(uint32_t crc, const uint8_t *buf, size_t len) {
    static int initialized = 0;
    if (!initialized) {
        make_crc_table(crc32c_table, CRCPOLY_CRC32C);
        initialized = 1;
    }
    return crc32_using_table(crc, buf, len, crc32c_table);
}

/* CRC-32 (BZIP2) */
uint32_t ZEXPORT crc32_bzip2(uint32_t crc, const uint8_t *buf, size_t len) {
    static int initialized = 0;
    if (!initialized) {
        make_crc_table(crc32_bzip2_table, CRCPOLY_BZIP2);
        initialized = 1;
    }
    return crc32_using_table(crc, buf, len, crc32_bzip2_table);
}

/* Combine two CRC values */
uint32_t ZEXPORT crc32_combine(uint32_t crc1, uint32_t crc2, z_off_t len2) {
    /* ... (existing combine implementation with modern types) ... */
}
/* File: crc32.c_chunk2 */

#include "zutil.h"
#include "crc32.h"
#include <inttypes.h>  // For PRIx32/64 macros (if needed)

local void write_table(FILE *out, const z_crc_t FAR *table)
{
    int n;

    for (n = 0; n < 256; n++)
        fprintf(out, "%s0x%08" PRIx32 "%s",
                (n % 4 == 0) ? "    " : ", ",
                (uint32_t)(table[n]),
                (n % 4 == 3) ? ",\n" : "");
}

local void write_table32hi(FILE *out, const z_crc_t FAR *table)
{
    int n;
    /* Ensure 64-bit type for shifting; safe even if z_crc_t is 32-bit */
    typedef uint64_t z_crc64_t;

    for (n = 0; n < 256; n++)
        fprintf(out, "%s0x%08" PRIx32 "%s",
                (n % 4 == 0) ? "    " : ", ",
                (uint32_t)((z_crc64_t)(table[n]) >> 32),
                (n % 4 == 3) ? ",\n" : "");
}

/* Original CRC-32 table and function definitions follow */
    /* Prepare to compute the CRC on full 64-bit words word[0..num-1]. */
    word = (z_word_t const *)buf;
    num = len >> 3;
    len &= 7;

    /* Do three interleaved CRCs to realize the throughput of one crc32x
       instruction per cycle. Each CRC is calculated on Z_BATCH words. The
       three CRCs are combined into a single CRC after each set of batches. */
    while (num >= 3 * Z_BATCH) {
        crc1 = 0;
        crc2 = 0;
        for (i = 0; i < Z_BATCH; i++) {
            val0 = word[i];
            val1 = word[i + Z_BATCH];
            val2 = word[i + 2 * Z_BATCH];
            __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc) : "r"(val0));
            __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc1) : "r"(val1));
            __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc2) : "r"(val2));
        }
        word += 3 * Z_BATCH;
        num -= 3 * Z_BATCH;
        crc = multmodp(Z_BATCH_ZEROS, crc) ^ crc1;
        crc = multmodp(Z_BATCH_ZEROS, crc) ^ crc2;
    }

    /* Do one last smaller batch with the remaining words, if there are enough
       to pay for the combination of CRCs. */
    last = num / 3;
    if (last >= Z_BATCH_MIN) {
        last2 = last << 1;
        crc1 = 0;
        crc2 = 0;
        for (i = 0; i < last; i++) {
            val0 = word[i];
            val1 = word[i + last];
            val2 = word[i + last2];
            __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc) : "r"(val0));
            __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc1) : "r"(val1));
            __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc2) : "r"(val2));
        }
        word += 3 * last;
        num -= 3 * last;
        val = x2nmodp(last, 6);
        crc = multmodp(val, crc) ^ crc1;
        crc = multmodp(val, crc) ^ crc2;
    }

    /* Compute the CRC on any remaining words. */
    for (i = 0; i < num; i++) {
        val0 = word[i];
        __asm__ volatile("crc32x %w0, %w0, %x1" : "+r"(crc) : "r"(val0));
    }
    word += num;

    /* Complete the CRC on any remaining bytes. */
    buf = (const unsigned char FAR *)word;
    while (len) {
        len--;
        val = *buf++;
        __asm__ volatile("crc32b %w0, %w0, %w1" : "+r"(crc) : "r"(val));
    }

    /* Return the CRC, post-conditioned. */
    return crc ^ 0xffffffff;
}

#else

#ifdef W

/*
  Return the CRC of the W bytes in the word_t data, taking the
  least-significant byte of the word as the first byte of data, without any pre
  or post conditioning. This is used to combine the CRCs of each braid.
 */
local z_crc_t crc_word(z_word_t data) {
    int k;
    for (k = 0; k < W; k++)
        data = (data >> 8) ^ crc_table[data & 0xff];
    return (z_crc_t)data;
}

local z_word_t crc_word_big(z_word_t data) {
    int k;
    for (k = 0; k < W; k++)
        data = (data << 8) ^
            crc_big_table[(data >> ((W - 1) << 3)) & 0xff];
    return data;
}

#endif

/* ========================================================================= */
unsigned long ZEXPORT crc32_z(unsigned long crc, const unsigned char FAR *buf,
                              z_size_t len) {
    /* Return initial CRC, if requested. */
    if (buf == Z_NULL) return 0;

#ifdef DYNAMIC_CRC_TABLE
    once(&made, make_crc_table);
#endif /* DYNAMIC_CRC_TABLE */

    /* Pre-condition the CRC */
    crc = (~crc) & 0xffffffff;

#ifdef W

    /* If provided enough bytes, do a braided CRC calculation. */
    if (len >= N * W + W - 1) {
        z_size_t blks;
        z_word_t const *words;
        unsigned endian;
        int k;

        /* Compute the CRC up to a z_word_t boundary. */
        while (len && ((z_size_t)buf & (W - 1)) != 0) {
            len--;
            crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
        }

        /* Compute the CRC on as many N z_word_t blocks as are available. */
        blks = len / (N * W);
        len -= blks * N * W;
        words = (z_word_t const *)buf;

        /* Do endian check at execution time instead of compile time, since ARM
           processors can change the endianness at execution time. If the
           compiler knows what the endianness will be, it can optimize out the
           check and the unused branch. */
        endian = 1;
        if (*(unsigned char *)&endian) {
            /* Little endian. */

            z_crc_t crc0;
            z_word_t word0;
#if N > 1
            z_crc_t crc1;
            z_word_t word1;
#if N > 2
            z_crc_t crc2;
            z_word_t word2;
#if N > 3
            z_crc_t crc3;
            z_word_t word3;
#if N > 4
            z_crc_t crc4;
            z_word_t word4;
#if N > 5
            z_crc_t crc5;
            z_word_t word5;
#endif
#endif
#endif
#endif
#endif

            /* Initialize the CRC for each braid. */
            crc0 = crc;
#if N > 1
            crc1 = 0;
#if N > 2
            crc2 = 0;
#if N > 3
            crc3 = 0;
#if N > 4
            crc4 = 0;
#if N > 5
            crc5 = 0;
#endif
#endif
#endif
#endif
#endif

            /*
              Process the first blks-1 blocks, computing the CRCs on each braid
              independently.
             */
            while (--blks) {
                /* Load the word for each braid into registers. */
                word0 = crc0 ^ words[0];
#if N > 1
                word1 = crc1 ^ words[1];
#if N > 2
                word2 = crc2 ^ words[2];
#if N > 3
                word3 = crc3 ^ words[3];
#if N > 4
                word4 = crc4 ^ words[4];
#if N > 5
                word5 = crc5 ^ words[5];
#endif
#endif
#endif
#endif
#endif
                words += N;

                /* Compute and update the CRC for each word. The loop should
                   get unrolled. */
                crc0 = crc_braid_table[0][word0 & 0xff];
#if N > 1
                crc1 = crc_braid_table[0][word1 & 0xff];
#if N > 2
                crc2 = crc_braid_table[0][word2 & 0xff];
#if N > 3
                crc3 = crc_braid_table[0][word3 & 0xff];
#if N > 4
                crc4 = crc_braid_table[0][word4 & 0xff];
#if N > 5
                crc5 = crc_braid_table[0][word5 & 0xff];
#endif
#endif
#endif
#endif
#endif
                for (k = 1; k < W; k++) {
                    crc0 ^= crc_braid_table[k][(word0 >> (k << 3)) & 0xff];
#if N > 1
                    crc1 ^= crc_braid_table[k][(word1 >> (k << 3)) & 0xff];
#if N > 2
                    crc2 ^= crc_braid_table[k][(word2 >> (k << 3)) & 0xff];
#if N > 3
                    crc3 ^= crc_braid_table[k][(word3 >> (k << 3)) & 0xff];
#if N > 4
                    crc4 ^= crc_braid_table[k][(word4 >> (k << 3)) & 0xff];
#if N > 5
                    crc5 ^= crc_braid_table[k][(word5 >> (k << 3)) & 0xff];
#endif
#endif
#endif
#endif
#endif
                }
            }

            /*
              Process the last block, combining the CRCs of the N braids at the
              same time.
             */
            crc = crc_word(crc0 ^ words[0]);
#if N > 1
            crc = crc_word(crc1 ^ words[1] ^ crc);
#if N > 2
            crc = crc_word(crc2 ^ words[2] ^ crc);
#if N > 3
            crc = crc_word(crc3 ^ words[3] ^ crc);
#if N > 4
            crc = crc_word(crc4 ^ words[4] ^ crc);
#if N > 5
            crc = crc_word(crc5 ^ words[5] ^ crc);
#endif
#endif
#endif
#endif
#endif
            words += N;
        }
        else {
            /* Big endian. */

            z_word_t crc0, word0, comb;
#if N > 1
            z_word_t crc1, word1;
#if N > 2
            z_word_t crc2, word2;
#if N > 3
            z_word_t crc3, word3;
#if N > 4
            z_word_t crc4, word4;
#if N > 5
            z_word_t crc5, word5;
#endif
#endif
#endif
#endif
#endif

            /* Initialize the CRC for each braid. */
            crc0 = byte_swap(crc);
#if N > 1
            crc1 = 0;
#if N > 2
            crc2 = 0;
#if N > 3
            crc3 = 0;
#if N > 4
            crc4 = 0;
#if N > 5
            crc5 = 0;
#endif
#endif
#endif
#endif
#endif

            /*
              Process the first blks-1 blocks, computing the CRCs on each braid
              independently.
             */

/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-2022 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
 * This interleaved implementation of a CRC makes use of pipelined multiple
 * arithmetic-logic units, commonly found in modern CPU cores. It is due to
 * Kadatch and Jenkins (2010). See doc/crc-doc.1.0.pdf in this distribution.
 */

#include "zutil.h"
#include "crc32.h"
#include "crc32_combine.h"
#include <stddef.h>  /* for ptrdiff_t */

local unsigned long crc32_combine_(unsigned long crc1, unsigned long crc2,
                                   z_off64_t len2);

/* Multiply two matrices in GF(2) for CRC combination. */
local void matmult(unsigned long a, unsigned long b, unsigned long *c) {
    unsigned long n;
    n = (a & 1) * b;
    if (a & 0x80000000UL)
        n ^= b << 31;
    a >>= 1;
    if (a) {
        matmult(a, b << 1, &n);
        if (a & 1)
            n ^= b;
    }
    *c = n;
}

/* Raise a matrix to the given power in GF(2) for CRC combination. */
local void matpow(unsigned long a, ptrdiff_t n, unsigned long *power) {
    unsigned long sq[2];
    do {
        if (n & 1) {
            matmult(a, a, sq);
            matmult(sq, *power, &a);
        }
        n >>= 1;
        if (n == 0)
            break;
        matmult(*power, *power, sq);
        *power = sq[0];
    } while (1);
}

/* Generate the matrix for combining CRCs over a given length. */
local unsigned long crc32_combine_gen_(ptrdiff_t len) {
    unsigned long M[2] = {0x80000000UL, 0};  /* Identity matrix */
    ptrdiff_t n = 0;

    if (len == 0)
        return M[0];  /* Return identity matrix for zero length */

    while ((len << n) < (sizeof(unsigned long) << 5))
        n++;
    n--;

    unsigned long mat = 0x80000000UL;  /* CRC polynomial */
    matpow(M[0], n - 1, &mat);
    matmult(M[0], mat, &M[0]);
    return M[0];
}

/* Combine two CRCs using the generated matrix. */
local unsigned long crc32_combine_(unsigned long crc1, unsigned long crc2,
                                   z_off64_t len2) {
    /* Handle zero length case */
    if (len2 == 0)
        return crc1;

    unsigned long M = crc32_combine_gen_(len2);
    unsigned long crc;

    matmult(M, crc1, &crc);
    return crc ^ crc2;
}

/* External interfaces */
unsigned long Z_EXPORT crc32_combine(unsigned long crc1, unsigned long crc2,
                                     z_off_t len2) {
    return crc32_combine_(crc1, crc2, len2);
}

unsigned long Z_EXPORT crc32_combine64(unsigned long crc1, unsigned long crc2,
                                      z_off64_t len2) {
    return crc32_combine_(crc1, crc2, len2);
}