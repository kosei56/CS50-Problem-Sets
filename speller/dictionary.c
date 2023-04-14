// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
int counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int index = hash(word);
    node *p = table[index];
    while (p != NULL)
    {
        if (strcasecmp(p->word, word) == 0)
        {
            return true;
        }
        else
        {
            p = p->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO'
    FILE *dicFile = fopen(dictionary, "r");
    if (dicFile == NULL)
    {
        printf("Couldn't open dictionary file\n");
        return false;
    }
    char *buffer = malloc(sizeof(char) * LENGTH);
    while (fscanf(dicFile, "%s", buffer) != EOF)
    {
        counter++;
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Not enough memory\n");
            return false;
        }
        strcpy(n->word, buffer);
        unsigned int index = hash(buffer);
        n->next = table[index];
        table[index] = n;
    }
    free(buffer);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *tmp;
    node *p;
    for (int i = 0; i < N; i++)
    {
        tmp = table[i];
        p = tmp->next;
        while (p == NULL)
        {
            free(tmp);
            tmp = p;
            p = tmp->next;
        }

    }
    return true;
}
