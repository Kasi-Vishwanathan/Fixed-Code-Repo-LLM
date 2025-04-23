#include <stdio.h>

int main() {
    int i;
    for (i = 0; i < 10; i++) { // Added missing semicolon
        printf("%d ", i);
    }
    return 0;
}