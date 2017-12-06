
/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a sorting algorithm
    int start;
    int middle;
    int end;

    start = 0;
    end = n - 1;

    // WHILE LENGTH OF LIST < 0
    while (start <= end)
    {
        //LOOK AT MIDDLE OF LIST
        middle = (start + end) / 2;
        // IF NUMBER FOUND RETURN TRUE
        if (value == values[middle])
        {
            return true;
        }

        // IF NUMBER HIGHER SEARCH LEFT
        else if (value < values[middle])
        {
            end = middle - 1;
        }

        // IF NUMBER LOWER SEARCH RIGHT
        else if (value > values[middle])
        {
            start = middle + 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // ITERATE THROUGH EACH ELEMENT FROM FIRST TO LAST (BY INCREMENTING IN ORDER FROM BEGINNING TO END)
    for (int i = 0; i < n; i++)
    {
        // ASSIGN I AS THE SMALLES ELEMENT
        int min = i;
        // FIND SMALLEST ELEMENT IN ARRAY(J) AND ASSIGNS IT AS MIN
        for (int j = i + 1; j < n; j++)
        {
            if (values[j] < values[i])
                min = j;
        }
        // SWAP SMALLEST ELEMENT WITH I
        int swap = values[i];
        values[i] = values[min];
        values[min] = swap;
    }
    return;
}
