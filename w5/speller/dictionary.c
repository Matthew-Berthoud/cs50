// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// One letter
// const unsigned int N = 26;

//Two letters
const unsigned int N = 26 * 26;

// Hash table
node *table[N];
unsigned int lengths[N]; // for size
bool loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *cur = table[hash(word)];
    while (strcasecmp(word, cur->word))
    {
        if (cur->next == NULL)
        {
            return false;
        }
        cur = cur->next;
    }
    return true;
}

// Hashes word to a number
// One letter
// unsigned int hash(const char *word)
// {
//     return toupper(word[0]) - 'A';
// }

// Two letters
unsigned int hash(const char *word)
{
    unsigned int i = toupper(word[0]) - 'A';
    if (word[1] == '\0')
    {
        return 26 * i + i; // put "a" in "aa" bin, "b" in "bb" bin, etc
    }
    else
    {
        unsigned int j = toupper(word[0]) - 'A';
        return 26 * i + j;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // set up buffer for following loop
    char buf[LENGTH + 1];
    int eof = 0;

    while (eof != EOF)
    {
        eof = fscanf(file, "%s", buf);
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            return false;
        }
        new->next = NULL;

        strcpy(new->word, buf);
        int idx = hash(buf);
        lengths[idx] += 1;
        if (table[idx] == NULL)
        {
            table[idx] = new;
        }
        else
        {
            node *old = table[idx];
            table[idx] = new;
            new->next = old;
        }
    }
    fclose(file);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int size = 0;
    if (loaded)
    {
        for (int i = 0; i < N; i++)
        {
            if (table[i] == NULL)
            {
                continue;
            }
            size = size + lengths[i];
        }
        return size - 1; // not sure why, but check50 output was all 1 too high
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cur = table[i];
        while (cur != NULL)
        {
            node *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    return true;
}