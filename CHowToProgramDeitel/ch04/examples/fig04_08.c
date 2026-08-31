#include<stdio.h>

int main(void) {
    for(int i=0; i<10; i++) {
        if(i == 5) continue; //skip 5
        printf("%d\n", i);
    }
    return 0;
}