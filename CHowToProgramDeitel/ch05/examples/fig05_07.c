//Game of craps
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

enum Status {CONTINUE, WON, LOST};
int rollDice(void);

int main(void) {
    srand(time(NULL));

    int myPoint = 0;
    enum Status gameStatus = CONTINUE;
    int sum = rollDice();

    switch(sum) {
        case 7:
        case 11:
            gameStatus = WON;
            break;
        case 2:
        case 3:
        case 12:
            gameStatus = LOST;
            break;
        default:
            gameStatus = CONTINUE;
            myPoint = sum;
            printf("Point is: %d\n",myPoint);
            break;
    }

    while(CONTINUE == gameStatus) {
        sum = rollDice();
        
        if(sum == myPoint) {
            gameStatus = WON;
        } else if(7 == sum) {
            gameStatus = LOST;
        }
    }

    if(WON == gameStatus) {
        printf("* Player WINS *\n");
    } else {
        printf("* Player LOSES *\n");
    }
}

int rollDice(void) {
    int die1 = 1 + (rand() % 6);
    int die2 = 1 + (rand() % 6);

    int sum = die1 + die2;
    printf("Player rolled %d and %d, sum is %d\n", die1, die2, sum);

    return sum;
}

