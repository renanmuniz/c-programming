// For a circle of radius 2, display the diameter, circumference and area. 
// Use the value 3.14159 for π. Use the following formulas
// (r is the radius): diameter = 2r, circumference = 2πr and area = πr2. Perform each
// of these calculations inside the printf statement(s) and use the conversion specification
// %f. This chapter discussed only integer constants and variables.
#include <stdio.h>

int main(void) {
    float PI = 3.14159;
    float radius = 0.0f;

    printf("%s", "Enter the radius of a circle: ");
    scanf("%f", &radius);

    float diameter = 2 * radius;
    float circumference = 2 * PI * radius;
    float area = PI * (radius * radius);
    
    printf("Diameter = %.2f\n", diameter);
    printf("Circumference = %.2f\n", circumference);
    printf("Area = %.2f\n", area);

    return 0;
}