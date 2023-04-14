#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool only_digits(string str);
char rotate(char letter, int key);

int main(int argc, string argv[])
{
    if(argc != 2 || !only_digits(argv[1]))
    {
        printf("Usage: ./cesar key\n");
        return 1;
    }
    int key = atoi(argv[1]);
    string plainText = get_string("plaintext:  ");
    printf("ciphertext: ");
    for(int i = 0, n = strlen(plainText); i < n; i++)
    {
        printf("%c", rotate(plainText[i], key));
    }
    printf("\n");
}

bool only_digits(string str)
{
    // 49 - 57
    int counter = 0;
    for(int i = 0, n = strlen(str); i < n; i++)
    {
        for(int j = 48; j < 58; j++)
        {
            if(str[i] == j)
            {
                counter++;
            }
        }
    }
    if(counter == strlen(str))
    {
        return true;
    }
    return false;
}

char rotate(char letter, int key)
{
    if(isupper(letter) || islower(letter))
    {
        if(isupper(letter))
        {
            letter = (letter - 65 + key)%26 + 65;
        }
        if(islower(letter))
        {
            letter = (letter - 97 + key)%26 + 97;
        }
    }
    return letter;
}
