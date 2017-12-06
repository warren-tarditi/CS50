#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // ENSURES ARGC (ARGUMENT COUNT) IS EQUAL ONLY TO 2
    if (argc != 2)
    {
        printf("PLEASE ENTER SINGLE COMMAND LINE ARGUMENT\n");
        return 1;
    }

    {   // FROM STRING TO INTEGER
        string k = argv[1];
        int p = atoi(k);

        //GETS PLAINTEXT FROM USER TO CIPHER
        printf ("plaintext: ");
        string plaintext = get_string();
        printf("ciphertext: ");

        // ITERATE OVER EACH CHARACTER ONCE WITH A 'FOR' LOOP
        for (int i = 0, n = strlen(plaintext); i < n; i++)
        {
            // CONVERTING ALPHABET TO CIPHERTEXT EQUATION WITH UPPERCASE AND LOWER CASE LETTERS
            if (isupper(plaintext[i]))
            {
                printf ("%c", (plaintext[i] - 65 + p) % 26 + 65);
            }
            else if (islower(plaintext[i]))
            {
                printf ("%c", (plaintext[i] - 97 + p) % 26 + 97);
            }
            else
            // IF NOT IN ALPHABET THEN JUST PRINT IT AS IT IS
            {
                 printf ("%c", plaintext[i]);
            }

        }
    }
    printf ("\n");
}
