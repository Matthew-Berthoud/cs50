#include <cs50.h>
#include <stdio.h>
#include <string.h> // strlen(); strcat()
#include <ctype.h> // isdigit(); isalpha()
#include <stdlib.h> // atoi()

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    string key = argv[1];
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (isdigit(key[i]) == 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    int k = atoi(key);
    string plain = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        char ch = plain[i];
        int val = ch;
        if isupper(ch)
        {
            val = (val - 65 + k) % 26 + 65;
        }
        else if islower(ch)
        {
            val = (val - 97 + k) % 26 + 97;
        }
        char new_ch = val;
        printf("%c", new_ch);
    }
    printf("\n");
}