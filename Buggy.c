/* File: Buggy.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Hypothetical original problematic code might have looked like:
   
   int process_data(buf, size)
   char *buf;
   int size;
   {
       ...
   }
*/

// Modern function prototype with ANSI C syntax
static int process_data(char *buf, size_t size)
{
    if (buf == NULL || size == 0) {
        fprintf(stderr, "Invalid input parameters\n");
        return -1;
    }
    
    // Process data safely
    return 0;
}

// Modern main function prototype
int main(void)
{
    const size_t BUF_SIZE = 1024;
    char *buffer = malloc(BUF_SIZE);
    
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    if (process_data(buffer, BUF_SIZE) != 0) {
        free(buffer);
        return EXIT_FAILURE;
    }
    
    free(buffer);
    return EXIT_SUCCESS;
}