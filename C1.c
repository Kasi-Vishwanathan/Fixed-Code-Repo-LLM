/* C1.c */
#include <stdio.h>
#include <stdlib.h>

/* Original K&R-style declarations */
void* zcalloc();
void zcfree();

/* Modernized declarations */
void* zcalloc(unsigned items, unsigned size);
void zcfree(void* ptr);

int main(void) {
    unsigned items = 10;
    unsigned size = sizeof(int);
    int* array = (int*)zcalloc(items, size);
    
    if (array) {
        printf("Memory allocation successful\n");
        zcfree(array);
    } else {
        printf("Memory allocation failed\n");
    }
    
    return 0;
}

void* zcalloc(unsigned items, unsigned size) {
    return calloc(items, size);
}

void zcfree(void* ptr) {
    free(ptr);
}
