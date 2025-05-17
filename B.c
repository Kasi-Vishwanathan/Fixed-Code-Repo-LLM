#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

// Modern ANSI C prototype with const correctness
void write_message(const char *msg) {
    printf("%s\n", msg);
}

// Updated to ANSI prototype and safe string concatenation
char* concat(const char *str1, const char *str2, size_t *buffer_size) {
    size_t needed = strlen(str1) + strlen(str2) + 1;
    if (needed > *buffer_size) {
        char *new_buf = realloc(buffer, needed);
        if (!new_buf) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        buffer = new_buf;
        *buffer_size = needed;
    }
    snprintf(buffer, *buffer_size, "%s%s", str1, str2);
    return buffer;
}

// Proper main declaration with error checking
int main(void) {
    size_t buffer_size = BUFFER_SIZE;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        perror("Initial malloc failed");
        return EXIT_FAILURE;
    }

    const char *part1 = "Hello, ";
    const char *part2 = "world!";
    
    char *result = concat(part1, part2, &buffer_size);
    write_message(result);
    
    free(buffer);
    return 0;
}
