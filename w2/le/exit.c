// exit status
    // main can signal success with its return value

#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Missing command-line argument\n");
        return 1; // returns from main that something is wrong
    }
    printf("hello, %s\n", argv[1]);
    return 0; // the 0 and 1 return values are also error codes, so you know
              // specifically what went wrong
}