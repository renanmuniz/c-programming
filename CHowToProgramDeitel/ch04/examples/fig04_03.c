#include<stdio.h>

int main(void) {
    int sum = 0;

    for(int number = 2; number <= 100; number += 2){
        sum += number;
    }

    printf("Sum is %d\n", sum);
    return 0;
}