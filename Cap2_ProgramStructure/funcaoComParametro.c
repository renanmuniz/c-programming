#include<stdio.h>

int soma(int n1, int n2) {
    return n1 + n2;
}

void printName(char* name) {
    printf("%s\n", name);
}


int main() {
    printf("Soma de 3 + 5 = %d\n",  soma(3,5));
    printName("Renan");
    return 0;
}