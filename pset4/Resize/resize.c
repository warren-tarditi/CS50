/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // - REMEMBER SIZE INCREASE FACTOR (N) - CONVERT FACTOR N TO INT
    int n = atoi(argv[1]);

    // - CHECK FACTOR IS WITHIN RANGE
    if (n < 1 || n > 100)
    {
        printf("factor must be in range\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER - SET NEW FILE HEADER
    BITMAPFILEHEADER bf;
    BITMAPFILEHEADER bf_new;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    bf_new = bf;

    // read infile's BITMAPINFOHEADER - SET NEW INFO HEADER
    BITMAPINFOHEADER bi;
    BITMAPINFOHEADER bi_new;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    bi_new = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //  - SET NEW DIMENSIONS FOR WIDTH AND HEIGHT BY MULTIPLYING OLD W AND H BY FACTOR
    bi_new.biWidth = bi.biWidth * n;
    bi_new.biHeight = bi.biHeight * n;

    //  - DETERMINE PADDING FOR SCANLINES
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //  - SET NEW IMAGE SIZE
    bi_new.biSizeImage = (bi_new.biWidth * sizeof(RGBTRIPLE) + new_padding)  * abs(bi_new.biHeight);
    bf_new.bfSize = bf.bfSize - bi.biSizeImage + bi_new.biSizeImage;

    // write outfile's BITMAPFILEHEADER - INSERT NEW BF AND BI HEADERS
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines - VERTICAL
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // - ITERATING EACH ROW (X) BY N NUMBER OF TIMES - HORIZONTAL
        for (int x = 0; x < n; x++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile * N NUMBER OF TIMES -
                for (int k = 0; k < n; k++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // - NEW PADDING
            for (int l = 0; l < new_padding; l++)
                fputc(0x00, outptr);

            // - GO BACK BEGINNING OF ROW - RE COPY METHOD BACK TO VERTICAL ITERATION
            if (x < (n - 1))
            fseek(inptr, - bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    // success
    return 0;
}