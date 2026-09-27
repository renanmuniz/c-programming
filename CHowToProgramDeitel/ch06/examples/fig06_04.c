#include<stdio.h>
#define SIZE 5 // define the size of the array with a constant value.

int main(void) {
    int n[SIZE] = {0}; // initialize all elements of the array to zero.
    int sum = 0;

    for(size_t i=0; i<SIZE; i++) {
        n[i] = 2 + 2 * i;
        sum = sum + n[i];
    }

    printf("%s%8s\n", "Element", "Value");

    for(size_t i=0; i<SIZE; i++) {
        printf("%7zu%8d\n", i, n[i]); // Output: 2 4 6 8 10
    }

    printf("Sum of all elements is: %d\n", sum); // Output: 30
}