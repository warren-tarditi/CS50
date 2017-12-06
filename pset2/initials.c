#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // GETS STRING AND MAKES SURE STRING IS VALID
    string s = get_string();
    if (s != NULL)

    // PRINTS CAPITAL LETTERS OF FIRST INITIAL
    printf("%c", toupper (s[0]));

        // FOR LOOP TO PRINT CAPITAL LETTER OF SECOND INITIAL AFTER SPACE ' '
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if (s[i] == ' ')
            {
                printf ("%c", toupper (s[i+1]));
            }
        }
    printf ("\n");
}