#include <stdio.h>

int min(int a, int b);
void test(void);

int min(int a, int b) {
    return (a < b) ? a : b;
}

void test(void) {
    printf("min: %d\n", min(10, 20));
}

int main(void) {
    test();
    return 0;
}
/* zlib.h is assumed to include necessary definitions (e.g., ct_data, MAX_BITS) */
#include "zutil.h"

void gen_codes(ct_data *tree, int max_code, const int *bl_count) {
    unsigned int next_code[MAX_BITS + 1]; /* Holds the next code for each bit length */
    unsigned int code = 0;
    
    /* Compute initial next_code values for each bit length */
    for (int bits = 1; bits <= MAX_BITS; ++bits) {
        next_code[bits] = code;
        code = (code + bl_count[bits - 1]) << 1;
    }

    /* Assign codes to all symbols in the tree */
    for (int n = 0; n <= max_code; ++n) {
        const int len = tree[n].Len;
        if (len != 0) {
            tree[n].Code = (unsigned short)next_code[len]++;
        }
    }
}
