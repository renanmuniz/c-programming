//Binary Search
#include<stdio.h>
#include<time.h>
#define SIZE 10000000

long binarySearch(const int array[], long key, size_t size);

int main(void) {
    static int a[SIZE] = {0};

    for(size_t x = 0; x < SIZE; x++) {
        a[x] = 2 * x;
    }

    printf("Enter integer search key: ");
    long searchKey = 0;
    scanf("%ld", &searchKey);

    clock_t start = clock();
    long subscript = binarySearch(a, searchKey, SIZE);
    clock_t end = clock();

    double elapsedMs = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    if(subscript != -1) {
        printf("Found value at subscript %ld\n", subscript);
    } else {
        puts("Value not found");
    }

    printf("Search took %f ms\n", elapsedMs);

    return 0;
}

// requires a sorted array
long binarySearch(const int array[], long key, size_t size) {
    long low = 0;
    long high = (long)size - 1;

    while(low <= high) {
        long middle = low + (high - low) / 2;

        if(array[middle] == key) {
            return middle;
        } else if(array[middle] > key) {
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }
    return -1;
}
