#include <cs50.h>
#include <stdio.h>

// argc : count the command line arguments
// argv : array of the command line arguments


int main(int argc, string argv[])
{
    if (argc == 2)
    {
        printf("hello, %s\n",argv[1]); // argv is an array of strings so it can use %s
    }
    else
    {
        printf("hello, world\n");
    }
}