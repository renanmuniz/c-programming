#include<stdio.h>

int main(void) {
    int students = 1;
    int passed = 0;
    int failed = 0;

    while(students <= 10) {
        printf("%s", "Enter the student situation(1 = Passed | 2 = Failed)");

        int situation = 0;
        scanf("%d", &situation);

        if(situation == 1) {
            passed++;
        } else if (situation == 2) {
            failed++;
        } else {
            printf("Invalid Input Value.\n");
            while (getchar() != '\n'); // consume bad input
            continue;
        }
        students++;
    }
    printf("PASSED: %d\n", passed);
    printf("FAILED: %d\n", failed);
    return 0; 
}