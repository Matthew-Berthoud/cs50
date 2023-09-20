#include <cs50.h>
#include <stdio.h>

int main(void)
{

    //Make variable n so we can use it in do-while loop
    int n;

    do
    {

        //Get height from user
        n = get_int("Height: ");
    }

    //Keep prompting for height if outside of 1-8
    while (n > 8 || n < 1);
    //Used to test do while functionality:
    //printf("%i\n",n);

    //Lines
    for (int i = 1; i <= n; i++)
    {
        //Spaces
        for (int k = n - i; k > 0; k--)
        {
            printf(" ");
        }

        //Hashtags
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}