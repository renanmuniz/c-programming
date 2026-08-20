// Write a program that reads two integers and determines and displays
// whether the first is a multiple of the second. Use the remainder operator.

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n1 = 0;
    int n2 = 0;

    printf("Enter the first integer number: ");
    scanf("%d", &n1);

    printf("Enter the second integer number: ");
    scanf("%d", &n2);

    if(n2 == 0) {
        printf("Cannot divide by zero.");
        exit(1);
    }

    if(n1 % n2 == 0) {
        printf("%d is multiple of %d", n1, n2);
    } else {
        printf("%d is NOT multiple of %d", n1, n2);
    }

    return 0;
}