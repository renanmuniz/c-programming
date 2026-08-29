// fig03_07.c
// Preincrementing and postincrementing.
#include<stdio.h>

int main(void) {
    int c = 1;

    printf("%d\n", c);    //1
    printf("%d\n", c++);  //1
    printf("%d\n", c);    //2
    printf("%d\n", ++c);  //3
    printf("%d\n", c);    //3

    return 0;
}