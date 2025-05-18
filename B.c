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
