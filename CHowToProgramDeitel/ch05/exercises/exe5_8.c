#include<stdio.h>
#include<math.h>

int main(void) {
    printf("%.2f\n", fabs(7.5));
    printf("%.2f\n", floor(7.5));
    printf("%.2f\n", fabs(0.0));
    printf("%.2f\n", ceil(0.0));
    printf("%.2f\n", fabs(-6.4));
    printf("%.2f\n", ceil(-6.4));
    printf("%.2f\n", ceil(-fabs(-8 + floor(-5.5))));
}