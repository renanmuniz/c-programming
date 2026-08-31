#include <stdio.h>

int main(void)
{
    int aCount = 0;
    int bCount = 0;
    int cCount = 0;
    int dCount = 0;
    int eCount = 0;
    int fCount = 0;

    printf("Enter the grades.\n");
    printf("Enter EOF(CTRL+D) to end input.\n");

    int grade;

    while ((grade = getchar()) != EOF)
    {
        switch (grade) {
            case 'A':
            case 'a':
                aCount++;
                break;
            case 'B':
            case 'b':
                bCount++;
                break;
            case 'C':
            case 'c':
                cCount++;
                break; 
            case 'D':
            case 'd':
                dCount++;
                break; 
            case 'E':
            case 'e':
                eCount++;
                break; 
            case 'F':
            case 'f':
                fCount++;
                break;
            case '\n': 
            case ' ': 
            case '\t':
                break;
            default:
                printf("%s", "Incorrect letter grade entered.\n");
                break;
        }
    }

    printf("\nTotals for each grade:\n");
    printf("A: %d\n", aCount);
    printf("B: %d\n", bCount);
    printf("C: %d\n", cCount);
    printf("D: %d\n", dCount);
    printf("E: %d\n", eCount);
    printf("F: %d\n", fCount);
}
