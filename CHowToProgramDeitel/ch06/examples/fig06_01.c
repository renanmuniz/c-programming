#include <stdio.h>

int main(void) {
    int n[5];
    for(int i=0; i<5; i++) {
        n[i] = 0;
    }

    printf("%s%8s\n", "Element", "Value");

    size_t length = sizeof(n) / sizeof(n[0]); // size of the array divided by size of one element

    for(size_t i=0; i<length; i++) {
        printf("%7zu%8d\n", i, n[i]);
    }

    return 0;
}