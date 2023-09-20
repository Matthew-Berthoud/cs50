#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
// Check for 2 args
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
// Check for 26 letters
    string key = argv[1];
    int n = strlen(key);
    if (n != 26)
    {
        printf("Key must contain 26 unique letters.\n");
        return 1;
    }
// Check that everything is a letter
    for (int i = 0; i < n; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must contain 26 unique letters.\n");
            return 1;
        }
// Check that letters after current aren't equal to it
        for (int j = i + 1; j < n; j++)
        {
            if (key[j] == key[i])
            {
                printf("Key must contain 26 unique letters.\n");
                return 1;
            }
        }
    }
// Get input string
    string plain = get_string("plaintext:  ");
    int length = strlen(plain);
    printf("ciphertext: ");
// Perform cipher and output cipher text
    for (int i = 0; i < length; i++)
    {
        char ch = plain[i];
        char end_ch = ch;
        int val = ch;
        if isupper(ch)
        {
            char new_ch = key[val - 65]; // index key list at correct letter
            end_ch = toupper(new_ch); // put back to uppercase
        }
        else if islower(ch)
        {
            char new_ch = key[val - 97]; // index key list at correct letter
            end_ch = tolower(new_ch); // put back to lowercase
        }
        printf("%c", end_ch);
    }
    printf("\n");
    return 0;
}