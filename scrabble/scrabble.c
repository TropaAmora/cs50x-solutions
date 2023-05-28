#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner based on their score
    if (score1 == score2)
    {
        printf("Tie!\n");
    }
    else
    {
        if (score1 > score2)
        {
            printf("Player 1 wins!\n");
        }
        else
        {
            printf("Player 2 wins!\n");
        }
    }
}

// function that gives the score based on the string promped by th user
int compute_score(string word)
{
    int sum = 0;
    int index = 0;
    int i = 0;
    // do this cycle until the end of the string, since the null value indicates its ending
    while (word[i] != 0)
    {
        // attribute the ascii value for each letter
        index = word[i];
        // if the value is a letter from the alphabet, the program regists its points, else the program goes to the next character
        if (((index < 91) && (index > 64)) || ((index < 123) && (index > 96)))
        {
            if (isupper(word[i]))
            {
                index = word[i] - 65;
                sum = sum + POINTS[index];
            }
            else
            {
                index = word[i] - 97;
                sum = sum + POINTS[index];
            }
        }
        i++;
    }
    return sum;
}
