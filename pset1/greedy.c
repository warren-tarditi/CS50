#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)

{
float money;

    //ASKS USER FOR AMOUNT OWED
    do
    {
        printf ("O hai! How much change is owed?\n");
        money = get_float();
    }
    while (money<0);

    // CONVERTS DOLLARS INTO CENTS


    int value = (money*1000)/10;
    int coins = 0;

    coins = value/25;
    value = value%25;
    coins = coins + value/10;
    value = value%10;
    coins = coins + value/5;
    value = value%5;
    coins = coins + value;

    // PRINTS OUT RESULT IN COINS BASED ON VARIABLES
    printf ("%i\n", coins);

}