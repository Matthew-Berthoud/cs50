// Debugging techniques / tools
    // use temporary print statements to just see what variables are
    // debug50 ./buggy
        // lets you step through your code
        // step into can go into function calls, etc.
    // rubber duck: talk it through! step by step


// don't think this file is fully debugged btw
// I think it needs to be while (n > 0);
#include <stdio.h>
#include <cs50.h>

int get_negative_int(void);

int main(void)
{
    int i = get_negative_int();
    printf("%i\n",i);
}

int get_negative_int(void)
{
    int n;
    do
    {
        n = get_int("Negative Integer: ");
        printf("n is %i\n", n);
    }
    while (n < 0);
    return n;
}