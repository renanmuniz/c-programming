#include<stdio.h>

int main(void) {
    int n[5]  = {1,2,3,4,5}; // initialize array with 5 elements directly in the declaration.

    size_t arraySize = sizeof(n) / sizeof(n[0]);

    printf("%s%8s\n", "Element", "Value");

    for(size_t i=0; i<arraySize; i++) {
        printf("%7zu%8d\n", i, n[i]);
    }
}