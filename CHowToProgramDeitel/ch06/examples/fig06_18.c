#include<stdio.h>

int main(void) {
    size_t columns = 0;
    size_t rows = 0;

    printf("%s", "Enter the number of columns the array will have: ");
    scanf("%zu", &columns);
    
    printf("%s", "Enter the number of rows the array will have: ");
    scanf("%zu", &rows);

    int array[rows][columns];
    int value = 0;

    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < columns; j++) {
            array[i][j] = value;
            value++;
        }
    }

    printf("%s", "Values of the array:\n");

    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < columns; j++) {
            printf("%d ", array[i][j]);
        }
        puts("");
    }

    puts("");
    printf("Checking the size of the array:\n");
    size_t nrows = sizeof array / sizeof array[0];
    size_t ncols = sizeof array[0] / sizeof array[0][0];

    printf("Columns: %zu\n", ncols);
    printf("Rows: %zu\n", nrows);
    
    return 0;
}