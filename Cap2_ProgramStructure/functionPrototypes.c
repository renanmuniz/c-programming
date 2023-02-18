#include<stdio.h>

//Prototipos de funcoes:
//São usados para declarar as funcoes para serem conhecidas pelo programa desde o inicio mas sem definir seu conteudo
int soma(int n1, int n2);
int subtracao(int n1, int n2);
int multiplicacao(int n1, int n2);
int divisao(int n1, int n2);


int main() {
    printf("%d\n",soma(2,3));
    printf("%d\n",subtracao(10,5));
    printf("%d\n",multiplicacao(2,10));
    printf("%d\n",divisao(9,3));
    return 0;
}


//Definindo as funcoes:
int soma(int n1, int n2) {
    return n1 + n2;
}

int subtracao(int n1, int n2) {
    return n1 - n2;
}

int multiplicacao(int n1, int n2) {
    return n1 * n2;
}

int divisao(int n1, int n2) {
    return n1 / n2;
}