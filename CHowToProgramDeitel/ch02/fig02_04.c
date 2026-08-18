#include<stdio.h>

int main(void) {
    int integer1 = 0;
    int integer2 = 0;

    printf("Enter first integer\n");
    scanf("%d", &integer1);
    printf("Enter second integer\n");
    scanf("%d", &integer2);

    printf("Sum is %d\n", integer1 + integer2);
}