#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(void) {
    // srand() seeds the random number generator. Without this, rand() would produce the same sequence of numbers each time the program runs.
    srand(time(NULL)); // Seed the random number generator with the current time. This way the numbers will be different each time the program runs.
    /*
    time(NULL) returns the current time as a time_t (seconds since the Unix epoch, Jan 1 1970). 
    Passing NULL means "I don't need the result stored in a variable, just return it" 
    (the function can optionally also write the value to a pointer you pass in, but that's rarely used). 
    That value changes every second, so using it as the seed for srand() makes the random sequence different 
    on each run — as opposed to a fixed seed like srand(1), which would produce the same sequence every time.
    */

    int frequency1 = 0; // rolled 1 counter
    int frequency2 = 0; // rolled 2 counter
    int frequency3 = 0; // rolled 3 counter
    int frequency4 = 0; // rolled 4 counter
    int frequency5 = 0; // rolled 5 counter
    int frequency6 = 0; // rolled 6 counter
    
    for(int i=0; i<100; i++) {
        int guess = 1 + rand() % 6;

        switch(guess) {
            case 1:
                frequency1++;
                break;
            case 2:
                frequency2++;
                break;
            case 3:
                frequency3++;
                break;
            case 4:
                frequency4++;
                break;
            case 5:
                frequency5++;
                break;
            case 6:
                frequency6++;
                break;
        }
    }

    printf("%s%13s\n", "Face","Frequency");
    printf(" 1 :%13d\n",frequency1);
    printf(" 2 :%13d\n",frequency2);
    printf(" 3 :%13d\n",frequency3);
    printf(" 4 :%13d\n",frequency4);
    printf(" 5 :%13d\n",frequency5);
    printf(" 6 :%13d\n",frequency6);

    return 0;
}