// Write a program that reads an integer and determines and displays
// whether it’s odd or even. Use the remainder operator. An even number is a multiple
// of two. Any multiple of two leaves a remainder of zero when divided by 2.

#include <stdio.h>

int main(void) {
    int n = 0;

    printf("%s", "Inform an integer number: ");
    scanf("%d", &n);

    if(n % 2 == 0) {
        printf("Number %d is even.\n", n);
    } else {
        printf("Number %d is odd.\n", n);
    }

    return 0;
}