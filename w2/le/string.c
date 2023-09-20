#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Input:  ");
    printf("Output: ");

    // this is the way we'd do it generally
    // printf("%s\n",s);

    // this is how we print string char by char
//    for (int i = 0; i< strlen(s); i++)
//    {
//        printf("%c", s[i]);
//    }
//    printf("\n");

// This isn't well designed because:
    // we keep checking the strlen() every iteration
    // store srtlen() as a variable to avoid this

for (int i = 0, n = strlen(s); i < n; i++) // we can declare variable within loop start
                                            // just like how we declare i
    {
        printf("%c", s[i]);
    }
    printf("\n");


}

