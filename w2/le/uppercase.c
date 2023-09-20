#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string("Before: ");
    printf("After:  ");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z') // you can directly write < and > with chars and such
        {                               // asking if the char is lowercase
            printf("%c", s[i] - 32);    // make it uppercase
        }
        else
        {
            printf("%c", s[i]);         // keep uppercase uppercase
        }
    }
    printf("\n");

// We reinvented the wheel again!!!
    // can use
    // if (islower(s[i]))
    // on line 12

    // for Booleans: 0 == False, any other int == True

// actually we DOUBLE reinvented the wheel!
    // function toupper() will do this for us as well
    // can put in loop and get rid of entire conditional structure

// NOTICE that these functions only work for CHARS, they're not STRING functions like in Python


}