#include<stdio.h>

int maximum(int n1, int n2, int n3);

int main(void) {
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;

    printf("%s", "Enter the 1st number: ");
    scanf("%d", &n1);

    printf("%s", "\nEnter the 2nd number: ");
    scanf("%d", &n2);

    printf("%s", "\nEnter the 3rd number: ");
    scanf("%d", &n3);

    printf("\nBiggest number is: %d\n", maximum(n1,n2,n3));

}

int maximum(int n1, int n2, int n3) {
    int maximum = n1;
    if(n2 > maximum) maximum = n2;
    if(n3 > maximum) maximum = n3;

    return maximum;
}