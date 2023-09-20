#include <cs50.h>
#include <stdio.h>
#include <string.h> // get_string(); strlen()
#include <ctype.h> // toupper()
#include <math.h> // round

int main(void)
{
    float letters = 0;
    float words = 1; // words == spaces + 1
    float sentences = 0;

// Didn't implement with functions. Faster to just loop through the text once
    string text = get_string("Text: ");
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i])) // character is a letter
        {
            letters++;
        }
        else if (text[i] == 32) // character is a space
        {
            words++;
        }
        else if (text[i] == 33 || text[i] == 46 || text[i] == 63) // '!', '.', '?'
        {
            sentences++;
        }
    }

    float L = letters / words * 100;
    float S = sentences / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int X = (int) round(index);

    if (X > 15)
    {
        printf("Grade 16+\n");
    }
    else if (X < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", X);
    }
}