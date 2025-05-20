/* a1.cpp */
#include <stdio.h>
#include <stdlib.h>

void process(arr, len)
int *arr;
int len;
{
    for (int i = 0; i <= len; ++i) {  // Bug: Off-by-one error
        arr[i] = i;
    }
}

int main() {
    int *data = (int*)malloc(10 * sizeof(int));
    process(data, 10);
    free(data);
    return 0;
}