#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n = get_int("How many scores? ");

    int scores[n];

    for (int i = 0; i < n; i++)
    {
        scores[i] = get_int("Score: ");
    }

    // if one value in a operation is a float, the result will be
    printf("Average: %f\n", (scores[0] + scores[1] + scores[2])/3.0);
}

// Arrays in C
    // one variable to store multiple values OF SAME TYPE
    // declare the array first with size
        // int name[size];
        // int scores[3]; // array called scores of size 3
    // then fill each cell:
        // scores[0] = 6 // don't need to say int again because we already put that
    // don't save space, just limit having tons of variable names