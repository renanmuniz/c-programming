// Write a program that reads two integers from the user
// then displays the larger number followed by the words “is larger.” If the numbers
// are equal, display the message “These numbers are equal.” Use only the single-selection
// form of the if statement you learned in this chapter.

#include <stdio.h>

int main(void) {
    int n1 = 0;
    int n2 = 0;

    printf("Enter the first integer: ");
    scanf("%d", &n1);

    printf("Enter the second integer: ");
    scanf("%d", &n2);

    if(n1 > n2) {
        printf("%d is larger.\n", n1);
    } else if(n2>n1) {
        printf("%d is larger.\n", n2);
    } else {
        printf("They are equal\n");
    }

    return 0;
}