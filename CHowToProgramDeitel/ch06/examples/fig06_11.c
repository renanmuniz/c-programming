#include<stdio.h>
#define SIZE 5

void modifyArray(int b[], size_t size);
void modifyElement(int e);

int main(void) {
    int a[SIZE] = {0,1,2,3,4};

    puts("Effects of passing entire array by reference:\n\nThe values of the original array are:");

    for(size_t i = 0; i < SIZE ; i++) {
        printf("%3d", a[i]);
    }

    puts("");

    modifyArray(a, SIZE);

    puts("The values of the modified array are:");

    for(size_t i = 0; i < SIZE; i++) {
        printf("%3d", a[i]);
    }

    printf("\n\n\nEffects of passing array element by value:\n\nThe value of a[3] is %d\n", a[3]);
    modifyElement(a[3]);

    printf("The value of a[3] is %d\n", a[3]);
    
    return 0;
}

void modifyArray(int b[], size_t size) {
    for(size_t i = 0; i < size; i++) {
        b[i] = b[i] * 2;
    }
}

void modifyElement(int e) {
    e = e * 2;
    printf("Value in modifyElement is: %d\n",e);
}

// void tryModifyArray(const int b[]) {
//     b[0] = b[0] * 2; // This will cause a compilation error because b is const
//     b[1] = b[1] * 2; // This will also cause a compilation error because b is const
//     b[2] = b[2] * 2; // This will also cause a compilation error because b is const
// }