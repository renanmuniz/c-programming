#include<stdio.h>

#define ROWS 3
#define COLUMNS 3

void printArrays(int a[ROWS][COLUMNS]);
void sumArrays(int a[ROWS][COLUMNS]);

int main(void) {
    int array[ROWS][COLUMNS] = {{1,2,3},{4,5,6},{7,8,9}};

    printArrays(array);
    sumArrays(array);

    return 0;
}

void printArrays(int a[ROWS][COLUMNS]) {
    for (size_t row = 0; row < ROWS; ++row) {
        for (size_t column = 0; column < COLUMNS; ++column) {
            printf("%d ", a[row][column]);
        }
        puts("");
    }
}

void sumArrays(int a[ROWS][COLUMNS]) {
    int sum = 0;

    for(size_t row = 0; row < ROWS; row++) {
        for(size_t column = 0; column < COLUMNS; column++) {
            sum = sum + a[row][column];
        }
    }

    printf("Sum is: %d", sum);
    puts("");
}