#include<stdio.h>

int main(void) {
    int big1 = 0;
    int big2 = 0;

    int counter = 1;
    while(counter <= 10){
        printf("\nInsert the #%d number: ", counter);
        
        int n = 0;
        scanf("%d",&n);

        if(n<0) {
            printf("\nMust be positive!");
            continue;
        }

        if(n > big1) {
            big2 = big1;
            big1 = n;
        } else if(n > big2) {
            big2 = n;
        }

        counter++;
    }
    printf("\nbig1: %d", big1);
    printf("\nbig2: %d", big2);

}