#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get user's name
    string name = get_string("Enter your name: ");

    //Print greeting
    printf("Hello, %s.\n", name);
}