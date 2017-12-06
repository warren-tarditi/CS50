#include <stdbool.h>
#include <stdio.h> // TO READ FROM FILE
#include <stdlib.h> // ALLOWS INTEGERS
#include <ctype.h> // ALLOWS ALPHABETIC LETTERS
#include <string.h>
#include "dictionary.h"

#define SIZE 1000000

typedef struct node
{
    char word[LENGTH + 1]; // LENGTH + \0
    struct node *next; // POINTER TO NEXT NODE
}
node;

//CREATE HASHTABLE
node* hashtable[SIZE] = {NULL};

// CREATE GLOBAL VARIABLE TO COUNT SIZE
int dicSize = 0;


// HASH FUNCTIONS IS USED TO DECIDE INTO WHICH LINKED LIST THAT WORD BELONGS
int hash(char* word)

    {
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % SIZE;
    }

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO creates a temp variable that stores a lower-cased version of the word
    char temp[LENGTH + 1];
    int len = strlen(word);


    for (int j = 0; j < len; j++)
    {
        temp[j] = tolower(word[j]);
    }
    temp[len] = '\0';


    //get the hash value of the passed in word
    int index = hash(temp);

    // if hashtable is empty at index, return false
    if(hashtable[index] == NULL)
    {
        return false;
    }

    // CREATE CURSOR TO COMPARE WITH WORD
    node *cursor = hashtable[index];

     // SEARCH THE WORD AND IF FOUND RETURN TRUE
    while (cursor != NULL)
    {
        if(strcmp (temp, cursor ->word) == 0)
            return true;
        cursor = cursor -> next;
    }
    return false;
}
/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // OPEN DITIONARY
    FILE* dic = fopen (dictionary, "r");
    if (dic == NULL)
        return false;

    // create an array for word to be stored in
    char word[LENGTH + 1];

    // scan the dictionary word by word
    while (fscanf(dic, "%s\n", word)!= EOF)
    {
        // increment dictionary size
        dicSize++;

        // malloc a new node for each word
        node* new_word = malloc(sizeof(node));

        // put word in the new node
        strcpy(new_word ->word, word);

        // find what index of the array the word should go in
        int index = hash(word);

         // if hashtable is empty at index, insert
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_word;
            new_word -> next = NULL;
        }
        // if hashtable is not empty at index, add at the end
        else
        {
            new_word -> next = hashtable[index];
            hashtable[index] = new_word;
        }
    }
    // close file
    fclose(dic);

    return true;
}
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return dicSize;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */

bool unload(void)
{
    //iterate through entire SIZE of the hashtable array
    for (int i = 0; i < SIZE; i++)
    {
        node* top = hashtable[i];

    // if hashtable is not empty, iterate through nodes and start freeing
        while (top != NULL)
        {
            node* tmp = top -> next;
            free(top);
            top = tmp;
        }
    }
    return true;
}