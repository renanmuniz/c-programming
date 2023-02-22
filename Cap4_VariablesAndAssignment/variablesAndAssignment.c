#include<stdbool.h>
#include<stdio.h>

#define ISSO_E_UMA_CONSTANTE 10; //Porem seu uso dessa forma nao é aconselhavel, usar a outra forma 'const';

// function prototypes
int somar(int n1, int n2);

int main(void)
{
    const int CONSTANTE_1 = 10;
    const int CONSTANTE_2 = 20;

    int aNumber = 10;
    long aBigNumber = 3211145;
    long long aReallyBigNumber = 425632238789;
    float inches = 33.0;
    float feet = 2.5;
    float yards = 1780;
    double length = 1.0 , width = 2.0 , height = 10.0;
    bool isItRaining = false;
    
    int myCounter = 0;
    int aDifferentCounter = myCounter;

    printf("Soma das constantes = %d\n", CONSTANTE_1 + CONSTANTE_2);

    int soma = 0;
    soma = somar(10,20);
    printf("Resultado da soma: %d\n", soma);

    return 0;
}

int somar(int n1, int n2) {
    int soma = n1 + n2;
    return soma;
}
