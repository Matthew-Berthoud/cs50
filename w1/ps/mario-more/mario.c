#include <cs50.h>
#include <stdio.h>

void print_hashes(int i);
void print_spaces(int i, int n);

int main(void)
{
    // This part same as mario-less
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n > 8 || n < 1);

    // Loop n times for rows
    for (int i = 1; i <= n; i++)
    {
        // Use functions to print out each part of row
        // First pyramid
        print_spaces(i, n);
        print_hashes(i);

        // Gap, second pyramid, new line
        printf("  ");
        print_hashes(i);
        printf("\n");
    }
}

// Similar logic to mario-less for these functions
void print_hashes(int i)
{
    for (int j = 0; j < i; j++)
    {
        printf("#");
    }
}

void print_spaces(int i, int n)
{
    for (int j = n - i; j > 0; j--)
    {
        printf(" ");
    }
}