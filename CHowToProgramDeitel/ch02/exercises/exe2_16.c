// Write a program that reads two integers from the user then displays
// their sum, product, difference, quotient and remainder.

#include <stdio.h>

int main(void)
{
    int n1 = 0;
    int n2 = 0;
    
    printf("Enter the first integer: ");
    scanf("%d", &n1);
    printf("Enter the second integer: ");
    scanf("%d", &n2);

    printf("Sum: %d\n", n1 + n2);
    printf("Product: %d\n", n1 * n2);
    printf("Difference: %d\n", n1 - n2);
    if (n2 != 0) {
        printf("Quotient: %d\n", n1 / n2);
        printf("Remainder: %d\n", n1 % n2);
    } else {
        printf("Cannot divide by zero.\n");
    }

    return 0;
}
