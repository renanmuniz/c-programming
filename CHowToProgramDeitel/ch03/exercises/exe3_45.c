#include<stdio.h>
#include<stdlib.h>

int main(void) {
    int n = 0;
    int factorial = 1;

    printf("Input the number to calculate the factorial: ");
    scanf("%d", &n);

    if(n<0) {
        printf("Number must be positive.");
        exit(0);
    }

    if(n == 0) {
        printf("Factorial is %d", factorial);
        exit(0);
    }

    while(n>=1) {
        factorial = factorial * n;
        n--;
    }

    printf("Factorial is: %d\n", factorial);
}