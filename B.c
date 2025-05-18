/* B.c */
#include <stdio.h>

long calc_sum(a, b, c)
int a, b;
double c;
{
    return a + b + c;
}

print_result(val)
long val;
{
    printf("Result: %ld\n", val);
}

main() {
    long sum = calc_sum(5, 3, 2.5);
    print_result(sum);
}