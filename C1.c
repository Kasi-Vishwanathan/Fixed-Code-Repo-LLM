// C1.c
#include <stdio.h>
#include <stdlib.h>

int sum(int *a, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) {  // Corrected loop condition
        s += a[i];
    }
    return s;
}

int main() {
    int n, *arr;
    printf("Enter size: ");
    scanf("%d", &n);
    if (n <= 0) {  // Check for valid size
        printf("Invalid size. Must be a positive integer.\n");
        return 1;
    }
    arr = malloc(sizeof(int) * n);
    if (arr == NULL) {  // Check allocation success
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int choice = 0;
    while (choice != 3) {
        printf("\n1.Sum  2.Print  3.Exit\n");
        scanf("%d", &choice);
        if (choice == 1) {
            printf("Sum = %d\n", sum(arr, n));
        } else if (choice == 2) {
            for (int i = 0; i < n; i++) {  // Corrected loop condition
                printf("%d ", arr[i]);
            }
            printf("\n");
        } else if (choice != 3) {
            printf("Bad option\n");
        }
    }
    free(arr);  // Free allocated memory
    return 0;
}
