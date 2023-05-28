#include <cs50.h>
#include <stdio.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    int i = 0;
    string text = get_string("Text: ");
    float letter_count = count_letters(text);
    float word_count = count_words(text);
    float sentences_count = count_sentences(text);

    // Coleman-Liau index equation
    float L = (letter_count / word_count) * 100.00;
    float S = (sentences_count / word_count) * 100.00;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    //Rating grades
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        if (index > 15)
        {
            printf("Grade 16+\n");
        }
        else
        {
            printf("Grade %i\n", index);
        }
    }
}

// function for counting letters
int count_letters(string text)
{
    int index = 0;
    int i = 0;
    int counter = 0;
    // run through the string
    while (text[i] != 0)
    {
        index = text[i];
        // check to see if it belongs to a ascii alphabet position
        if (((index < 91) && (index > 64)) || ((index < 123) && (index > 96)))
        {
            counter++;
        }
        i++;
    }
    return counter;
}

// function for counting words
int count_words(string text)
{
    int index = 0;
    int prev_index = 0;
    int i = 0;
    int counter = 0;
    // run through the string
    while (text[i] != 0)
    {
        index = text[i];
        // check to see if it is the end of a word based on the previous character
        if (((index == 32) || (index == 46) || (index == 63) || (index == 33) || (index == 59) || (index == 44)) && (((prev_index < 91)
                && (prev_index > 64)) || ((prev_index < 123) && (prev_index > 96))))
        {
            counter++;
        }
        i++;
        prev_index = index;
    }
    return counter;
}

// function for counting sentences
int count_sentences(string text)
{
    int index = 0;
    int prev_index = 0;
    int i = 0;
    int counter = 0;
    // run through the string
    while (text[i] != 0)
    {
        index = text[i];
        // check if the current character is a . ! ?
        if ((index == 46) || (index == 63) || (index == 33))
        {
            counter++;
        }
        i++;
        prev_index = index;
    }
    return counter;
}