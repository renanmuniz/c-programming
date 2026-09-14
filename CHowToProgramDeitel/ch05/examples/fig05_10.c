//recursion
//fibonacci sequence
// Example:
// fibonacci(5)
//     fibonacci(4) + fibonacci(3)
//         fibonacci(3) + fibonacci(2)
//             fibonacci(2) + fibonacci(1)
//                 fibonacci(1) + fibonacci(0)
// Fibonacci of 10 = 55

#include<stdio.h>

unsigned long long int fibonacci(int n);

int main(void) {
    int number = 0;

    printf("Enter the number to calculate it's fibonacci: ");
    scanf("%d", &number);

    printf("Fibonacci of %d is %llu\n", number, fibonacci(number));
}

unsigned long long int fibonacci(int n) {
    if(n == 0 || n == 1) {
        return n;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}