/* File: Buggy.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int process_data(char *buf, size_t size)
{
    if (buf == NULL || size == 0) {
        fprintf(stderr, "Invalid input parameters\n");
        return -1;
    }
    
    // Process data safely
    return 0;
}

int main(void)
{
    const size_t BUF_SIZE = 1024;
    char *buffer = malloc(BUF_SIZE);
    
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    // Initialize buffer to avoid potential undefined behavior when processing
    memset(buffer, 0, BUF_SIZE);
    
    if (process_data(buffer, BUF_SIZE) != 0) {
        free(buffer);
        return EXIT_FAILURE;
    }
    
    free(buffer);
    return EXIT_SUCCESS;
}
