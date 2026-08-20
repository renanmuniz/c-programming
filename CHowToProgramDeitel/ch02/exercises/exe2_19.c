// Write a program that inputs
// three different integers from the keyboard, then displays the sum, the average, the
// product, the smallest and the largest of these numbers. Use only the single-selection
// form of the if statement you learned in this chapter.

#include <stdio.h>

int main(void) {
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    
    int min = 0;
    int max = 0;

    printf("Enter the first integer: ");
    scanf("%d", &n1);
    min = n1;
    max = n1;

    printf("Enter the second integer: ");
    scanf("%d", &n2);
    if(n2 < min) min = n2;
    if(n2 > max) max = n2;

    printf("Enter the third integer: ");
    scanf("%d", &n3);
    if(n3 < min) min = n3;
    if(n3 > max) max = n3;

    printf("Sum: %d\n", n1 + n2 + n3);
    printf("Average: %d\n", (n1 + n2 + n3)/3);
    printf("Product: %d\n", n1 * n2 * n3);
    printf("Smallest: %d\n", min);
    printf("Biggest: %d\n", max);

    return 0;
}