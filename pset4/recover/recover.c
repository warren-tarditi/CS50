
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    // CHECK FOR PROPER COMMAND USAGE
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file\n");
        return 1;
    }
    // REMEMBER FILENAME
    char* infile = argv[1];

    // OPEN MEMORY CARD INPUT FILE
    FILE * inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr,"Could not open memory card, infile");
        return 2;
    }

    // SET BUFFER SIZE FROM 0 TO 512
    unsigned char buffer[512];
    // SET POINTER TO OUTFILE
    FILE* image = NULL;
    // SET COUNTER
    int count = 0;

    // READ UNTIL END OF CARD
    while(fread(buffer,512, 1, inptr) == 1)
    {
        // SEARCH THROUGH BEGGINING OF JPEG TO FIND START OF IMAGE
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3 >= 0xe0 && buffer[3] <= 0xef])
        {
            //IF FILE HAS BEEN FOUND CLOSE CURRENT IMAGE
            if (count > 0)
            {
                fclose(image);
            }

            // DECLARE FILENAME TO BE 8 CHARS LONG
            char filename[8];

            // CREATE NAME FOR IMAGE
            sprintf(filename, "%03i.jpg", count);
            image = fopen(filename, "a");
            count++;

        }
        // WRITE FILES 512 BYTES AT A TIME WHEN NEW JPG IS FOUND
        if (count > 0)
        {
            fwrite(buffer, 512, 1, image);
        }
    }
    fclose(inptr);
    fclose(image);
    return 0;
}

