#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool isKey(string str);
string substitute(string str, string key);
bool noRepeat(string key);

int main(int argc, string argv[])
{
    if(argc != 2 || !isKey(argv[1]) || !noRepeat(argv[1]))
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    string plainText = get_string("plaintext:  ");
    printf("ciphertext: ");
    printf("%s\n", substitute(plainText, argv[1]));
}

bool isKey(string str)
{
    int counter = 0;
    if(strlen(str) == 26)
    {
        for(int i = 0; i < 26; i++)
        {
            if(isupper(str[i]) || islower(str[i]))
            {
                counter++;
            }
        }
    }
    if(counter == 26)
    {
        return true;
    }
    return false;
}

string substitute(string str, string key)
{
    for(int i = 0, n = strlen(str); i < n; i++)
    {
        if(isupper(str[i]) || islower(str[i]))
        {
            if(isupper(str[i]))
            {
                str[i] = toupper(key[str[i] - 65]);
            }
            else if(islower(str[i]))
            {
                str[i] = tolower(key[str[i] - 97]);
            }
        }
    }
    return str;
}

bool noRepeat(string key)
{
    char checkLetter;
    for(int i = 0, n = strlen(key) - 1; i < n; i++)
    {
        checkLetter = key[i];
        for(int j = i + 1, t = strlen(key); j < t; j++)
        {
            if(checkLetter == key[j])
            {
                return false;
            }
        }
    }
    return true;
}