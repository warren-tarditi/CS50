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
    //LOOP OVER KEY
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        //ADVANCE TO THE NEXT LETTER IN KEYWORD ONLY IF CHARACTER IN PLAINTEXT IS LETTER
        if (isalpha(argv[1][i]) == 0)
        {
            printf("PLEASE INPUT LETTERS ONLY\n");
            return 1;
        }

    }
            //GETS PLAINTEXT FROM USER TO CIPHER
            printf ("plaintext: ");
            string plaintext = get_string();
            printf("ciphertext: ");

            string cipher = argv[1];
            int lenght = strlen(cipher);

        // LOOPS OVER EACH CHARACTER IN PLAINTEXT STRING
        for (int i = 0, j = 0, n = strlen(plaintext); i < n; i++)
        {
            // CALCULATE CIPHER LETTER USING FORMULA C1 = (P1 + KJ) % 26 THEN CONVERT TO ASCII NUMBER

            //FIRST FOR UPPER CASE OF KEY(CODE) AND TEXT(PLAINTEXT)
            if (isupper(plaintext[i]))
            {
                char code = toupper(cipher[j % lenght]);
                printf("%c", ((((plaintext[i] - 65) + (code - 65)) % 26) + 65));
                j++;
            }
            //THEN FOR LOWER CASE OF KEY(CODE) AND TEXT(PLAINTEXT)
            else if (islower(plaintext[i]))
            {
                char code = tolower(cipher[j % lenght]);
                printf("%c", ((((plaintext[i] - 97) + (code - 97)) % 26) + 97));
                j++;
            }

            //OTHERWISE PRINT IT AS IT IS
            else
            {
                printf("%c", plaintext[i]);
            }

        }

    printf("\n");
}