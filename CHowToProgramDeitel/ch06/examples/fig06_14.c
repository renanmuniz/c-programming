//Linear Search
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 10000000

long linearSearch(const int array[], long key, size_t size);

int main(void) {
    static int a[SIZE] = {0};

    for(size_t x = 0; x< SIZE; x++) {
        a[x]= 2 * x;
    }

    // Fisher-Yates shuffle so the data is unsorted
    srand((unsigned)time(NULL));
    for(size_t x = SIZE - 1; x > 0; x--) {
        size_t j = (size_t)rand() % (x + 1);
        int temp = a[x];
        a[x] = a[j];
        a[j] = temp;
    }

    printf("Enter integer search key: ");
    long searchKey = 0;
    scanf("%ld", &searchKey);

    clock_t start = clock();
    long subscript = linearSearch(a, searchKey, SIZE);
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

long linearSearch(const int array[], long key, size_t size) {
    for(size_t n = 0; n < size; n++) {
        if(array[n] == key) {
            return n;
        }
    }
    return -1;
}