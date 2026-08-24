#include <stdio.h>

int main(void) {
    int total = 0;
    int counter = 0;

    int grade = 0;
    printf("%s", "\nEnter the grade or -1 to exit: ");
    scanf("%d", &grade);

    while(grade != -1) {
        total = total + grade;
        counter = counter + 1;

        printf("%s", "\nEnter the next grade or -1 to exit: ");
        scanf("%d", &grade);
    }

    if(counter > 0) {
        double average = (double) total / counter;
        printf("\nThe average grade is: %.2f\n", average);
    } else {
        printf("%s", "\nNo grades were entered.\n");
    }
    return 0;
}