#include<stdio.h>
#include <stdbool.h>

//Logical operators
int main(void) {
    int age = 0;
    char gender = ' ';
    bool parentsPermission = false;
    
    printf("Enter the gender M/F:");
    gender = getchar();

    printf("Enter the age:");
    scanf("%d",&age);

    printf("Have parents permission? 1-Yes 2-No: ");
    int parentsPermissionInput = 0;
    int readSuccessfully = scanf("%d",&parentsPermissionInput);
    printf("Read successfully: %d\n", readSuccessfully);
    if(parentsPermissionInput == 1) {
        parentsPermission = true;
    } else {
        parentsPermission = false;
    }
    
    
    if(gender == 'M' && age >= 18 || parentsPermission) {
        printf("\nAllowed.\n");
    } else if(gender == 'F' && age >= 21 || parentsPermission) {
        printf("\nAllowed.\n");
    } else {
        printf("\nDenied.\n");
    }

    return 0;
}