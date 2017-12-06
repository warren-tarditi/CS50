#include <cs50.h>
#include <stdio.h>

int main(void)

{
    int min;

    //CONVERTS MINUTES AS NUMBERS
    printf("minutes:");
    min = get_int();

    //CAUSES OPERATOR TO RUN ONLY WITH POSITIVE INTEGER PROVIDED
    while(min<0);

    //CALCULATES AND GIVES RESULT OF VARIABLE N IN BOTTLES
    int n = min*12;
    printf("bottles:%i\n", n);
}