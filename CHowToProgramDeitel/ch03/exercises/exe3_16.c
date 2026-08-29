#include<stdio.h>

int main(void){
    double mileage = 0;
    double gallons = 0;

    double totalmileage = 0;
    double totalGallons = 0;

    int counter = 0;

    printf("%s", "Enter the mileage or -1 to exit.\n");
    scanf("%lf", &mileage);
    
    while(mileage != -1) {
        if(mileage > 0) {
            printf("%s", "Enter the consumed gallons.\n");
            scanf("%lf", &gallons);

            printf("Mileage for this tank was: %.2f miles per gallon\n", mileage/gallons);
            totalmileage += mileage;
            totalGallons += gallons;
            counter++;
        } else {
            printf("%s", "Values must be positive.\n");
        }

        printf("%s", "Enter the mileage or -1 to exit.\n");
        scanf("%lf", &mileage);
    }

    if(counter == 0) {
        printf("%s", "No mileage data was entered.\n");
    } else {
        printf("The total mileage was %.2f miles.\n", totalmileage);
        printf("The total gallons consumed was %.2f gallons.\n", totalGallons);
        printf("The average mileage was %.2f miles per gallon.\n", totalmileage/totalGallons);
    }
    return 0;
}