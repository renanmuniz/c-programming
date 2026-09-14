//Recursion
// Example:
// factorial(5)
//     5 * factorial(4)
//         4 * factorial(3)
//             3 * factorial(2)
//                 2 * factorial(1)

#include<stdio.h>

unsigned long long int factorial(int number);

int main(void) {
    for(int i = 0; i <= 21; i++) {
        printf("%d! = %llu\n", i, factorial(i));
    }
}

unsigned long long int factorial(int number) {
    if (number <= 1) {
        return 1;
    } else {
        return (number * factorial(number - 1));
    }
}