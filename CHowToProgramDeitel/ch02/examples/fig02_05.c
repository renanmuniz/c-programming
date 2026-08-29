#include<stdio.h>

int main(void) {
    printf("Enter two integers\n");
    int number1 = 0;
    int number2 = 0;
    scanf("%d %d", &number1, &number2);

    if(number1 == number2) {
        printf("%d == %d\n", number1, number2);
    }
    if(number1 != number2) {
        printf("%d != %d\n", number1, number2);
    }
    if(number1 < number2) {
        printf("%d < %d\n", number1, number2);
    }
    if(number1 > number2) {
        printf("%d > %d\n", number1, number2);
    }
    if(number1 <= number2) {
        printf("%d <= %d\n", number1, number2);
    }
    if(number1 >= number2) {
        printf("%d >= %d\n", number1, number2);
    }
    
    return 0;
}