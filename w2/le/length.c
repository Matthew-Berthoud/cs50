#include <stdio.h>
#include <cs50.h>
#include <string.h> // this gives us strlen()
                    // find what functions exist with cs50 manual pages

int main(void)
{
    string name = get_string("Name: ");

    // easy version
    int length = strlen(name);
    printf("%i\n",length);

    // hard work version
    int i = 0;
    while (name[i] != '\0')
    {
        i++;
    }
    printf("%i\n", i);

    // check if equal
    if (i == length)
    {
        printf("Equal\n");
    }
    else
    {
        printf("Didn't work, rewrite code!\n");
    }
}