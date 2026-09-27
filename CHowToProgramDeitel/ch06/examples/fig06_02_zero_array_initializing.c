#include<stdio.h>

int main(void) {
    int n[5]  = {1,2,3}; // initialize the array with informed values and the remainings are considerated ans zeros.

    size_t arraySize = sizeof(n) / sizeof(n[0]);

    printf("%s%8s\n", "Element", "Value");

    for(size_t i=0; i<arraySize; i++) {
        printf("%7zu%8d\n", i, n[i]); // Output: 1 2 3 0 0
    }
}