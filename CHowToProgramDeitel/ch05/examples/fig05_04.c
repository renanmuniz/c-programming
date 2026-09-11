#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    for(int i = 1; i <= 10; i++) {
        printf("%d \n", 1 + (rand() % 6));  //Need to add 1 because it starts from zero.
    }

    return 0;
}