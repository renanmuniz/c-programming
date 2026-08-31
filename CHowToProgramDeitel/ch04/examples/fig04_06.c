#include<stdio.h>

int main(void) {
    int counter = 1;

    do {
        printf("Counting... %d\n", counter);
        counter++;
    } while(counter <= 5);
}