#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    int space;
    int hash;

    do
    {
        printf("height: ");
        height = get_int();
    }

    while (height < 0 || height > 23);

    //GENERATES ROWS OF PYRAMID
    for (int i = 0; i < height; i++)

    {
        // LOOP FOR SPACES
        for (space = 0; space < (height -i-1); space++)
        {
        printf(" ");
        }

        // LOOP FOR HASH
        for (hash = 0; hash < (i+2); hash++)
        {
        printf("#");
        }
        printf("\n");
    }
}