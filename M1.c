#include <stdio.h>

int main() {
    int n, i;
    int max;
    int arr[10];

    printf("Enter number of elements (max 10): ");
    scanf("%d", &n);

    // Bug: No validation on n, could be > 10 or <= 0
    for (i = 1; i <= n; i++) {
        printf("Enter element %d: ", i);
        scanf("%d", &arr[i]);  // Bug: arr indexing starts at 1, should start at 0
    }

    max = arr[0];  // Bug: arr[0] might be uninitialized if n=0 or input invalid

    for (i = 0; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    printf("Maximum value is: %d\n", max);

    return 0;
}