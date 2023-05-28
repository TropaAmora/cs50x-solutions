#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string convertion(string plaintext, string cypher);

int main(int argc, string argv[])
{
    // checks the number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    {
        int length = strlen(argv[1]);
        // checks for the right length
        if (length != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            string cypher = argv[1];
            int i = 0;
            // runs throgth the cypher array to check for inconformaties
            while (i < 26)
            {
                int id = cypher[i];
                if (((id > 91) || (id < 64)) && ((id > 123) || (id < 96)))
                {
                    printf("Key must contain 26 different characters.\n");
                    return 1;
                }
                else
                {
                    for (int j = 0; j < i; j++)
                    {
                        if (id == cypher[j])
                        {
                            printf("Key must contain 26 different characters.\n");
                            return 1;
                        }
                    }
                }
                i++;
            }
            // prompts the user for the plaintext
            string plaintext = get_string("plaintext:  ");
            int n = strlen(plaintext);
            // creates an array to store the cyphertext characters, to then print them
            char cyphertext[n];
            i = 0;
            // runs through the plain text until the null character
            while (plaintext[i] != 0)
            {
                int index = plaintext[i];
                int id = 0;
                // checks if it is an alphabetic character
                if (((index < 91) && (index > 64)) || ((index < 123) && (index > 96)))
                {
                    // checks if it is upper or lower case
                    if (isupper(plaintext[i]))
                    {
                        id = index - 65;
                        // checks if the cypher value for i is upper case
                        if (isupper(cypher[id]))
                        {
                            cyphertext[i] = cypher[id];
                        }
                        // must be lower case
                        else
                        {
                            cyphertext[i] = cypher[id] - 32;
                        }
                    }
                    // must be lower case
                    else
                    {
                        id = index - 97;
                        // checks if the cypher value for i is upper
                        if (isupper(cypher[id]))
                        {
                            cyphertext[i] = cypher[id] + 32;
                        }
                        // must be lower case
                        else
                        {
                            cyphertext[i] = cypher[id];
                        }
                    }
                }
                else
                {
                    // if it is any ohter ascii value, the program does not change the int value
                    cyphertext[i] = plaintext[i];
                }
                i++;
            }
            printf("ciphertext: ");
            // run through the result array to print the ciphered text
            for (int j = 0; j < n; j++)
            {
                printf("%c", cyphertext[j]);
            }
            printf("\n");
            return 0;
        }
    }
}