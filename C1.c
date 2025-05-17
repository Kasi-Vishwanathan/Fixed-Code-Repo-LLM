// 50‑line Buggy C: Sum of array with naive menu
#include <stdio.h>
#include <stdlib.h>

int sum(int *a, int n) {              // BUG: no null‑check, no overflow guard
    int s = 0;
    for (int i = 0; i <= n; i++) {    // BUG: off‑by‑one (should be i < n)
        s += a[i];
    }
    return s;
}

int main() {
    int n, *arr;
    printf("Enter size: ");
    scanf("%d", &n);
    arr = malloc(sizeof(int) * n);    // BUG: malloc not checked for NULL

    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int choice = 0;
    while (choice != 3) {
        printf("\n1.Sum  2.Print  3.Exit\n");
        scanf("%d", &choice);
        if (choice == 1) {
            printf("Sum = %d\n", sum(arr, n));
        } else if (choice == 2) {
            for (int i = 0; i <= n; i++)          // BUG: prints one past end
                printf("%d ", arr[i]);
            printf("\n");
        } else if (choice != 3) {
            printf("Bad option\n");
        }
    }
    // BUG: forgot free(arr);
    return 0;
}
