#include<stdio.h>

int square(int number); //function prototype

int main(void) {
    for(int x = 1; x <= 10; x++) {
        printf("%d ", square(x));
    }

    return 0;
}

int square(int number) { //function definition
    return number * number;
}