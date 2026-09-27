#include<stdio.h>

int main(void) {
    double fixedPrice = 2.00;
    double addHourPrice = 0.50;
    double maxValue = 10.00;
    int maxParkingHours = 24;

    int parkingHours = 0;
    double valueToPay = 0.0;

    printf("%s", "Enter the number of parking hours: ");
    scanf("%d", &parkingHours);

    if(parkingHours > maxParkingHours) {
        printf("Max parking hours allowed is %dh.\n", maxParkingHours);
        return 0;
    }

    if(parkingHours <= 3) {
        valueToPay = fixedPrice;
    } else {
        valueToPay = fixedPrice + (parkingHours - 3) * addHourPrice;
    }

    if(valueToPay > maxValue) {
        printf("Total: $%.2f\n", maxValue);
    } else {
        printf("Total: $%.2f\n", valueToPay);
    }

    return 0;
}