// Implements a dictionary's functionality
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int counter = 0;
bool loaded = false;

// TODO: Choose number of buckets in hash table
const unsigned int N = 200000;
// the internet said it is a good rule of thumb to have a load factor of 70/75% (not accomplished with 200,000)

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // this index is the word
    int index = hash(word);
    node *ptemp = table[index];
    // goes through the linked list to the bucket in search of that word
    while (ptemp != NULL)
    {
        if (strcasecmp(word, ptemp->word) == 0)
        {
            return true;
        }
        ptemp = ptemp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    int word_length = strlen(word);
    // cycle through the word and first add the ascii value of each lowered character and then multiply it with the same value, but now
    //ensure that it is less than the maximum "storage value" in the table
    for (int i = 0; i < word_length; i++)
    {
        int temp = tolower(word[i]);
        hash_value = word_length;
        hash_value += temp;
        hash_value *= temp;
    }

    // return the value
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary) // *dictionary already is the pointer to the file considered
{
    // open the file and as7sign a pointer to it
    FILE *pfile = fopen(dictionary, "r");

    // check to see if the file opens, and print error message
    if (pfile == NULL)
    {
        printf("Unable to open the dictionary provided.\n");
        return false;
    }

    // create a string to store the read values
    char buffer[LENGTH + 1];

    // read one word at time - it could be done with a for (char c = getc(dictionary); c != EOF; c = getc(dictionary))
    while (fscanf(pfile, "%s", buffer) != EOF)
    {
        // create a new node for this word
        node *pword = malloc(sizeof(node));
        if (pword == NULL)
        {
            return false;
        }

        // copy the string present in dictionary to the buffer
        strcpy(pword->word, buffer);

        // Now we need to hash the value and store it in its place
        int hash_value = hash(buffer);
        pword->next = table[hash_value];
        table[hash_value] = pword;
        counter++;
    }

    // close file
    fclose(pfile);

    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // size is called after the dictionary is loaded
    if (!loaded)
    {
        return 0;
    }
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *pcursor = table[i];
        while (pcursor != NULL)
        {
            node *ptemp = pcursor;
            pcursor = pcursor->next;
            free(ptemp);

        }
        if (pcursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}