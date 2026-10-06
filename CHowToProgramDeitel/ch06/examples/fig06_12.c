#include<stdio.h>
#define SIZE 10

int main(void) {
    int a[SIZE] = {8,6,10,89,4,12,89,68,45,37};

    puts("Data in original order:");
    for(size_t i = 0; i < SIZE; i++) {
        printf("%4d ", a[i]);
    }
    puts("");

    printf("Sorting with bubble sort algorithm:");

    for(size_t pass = 0; pass < SIZE - 1; pass++) {
        for(size_t i = 0; i < SIZE - 1; i++) {
            if(a[i] > a[i + 1]) {
                int hold = a[i];
                a[i] = a[i + 1];
                a[i + 1] = hold;
            }
        }
    }

    puts("Data after bubble sort:");
    for(size_t i = 0; i < SIZE; i++) {
        printf("%4d ", a[i]);
    }
    puts("");


}