#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int counter = 0;
    int height = 0;
    // assures the code only accepts values between 1 and 8
    do
    {
        height = get_int("Height: ");
    }
    while ((height < 1) || (height > 8));

    //runs each line
    for (int i = 0; i < height; i++)
    {
        //runs for each column assigning the correct values, for more see OneNote
        for (int j = 0; j < height - 1 - counter; j++)
        {
            printf(" ");
        }
        for (int j = height - 1 - counter; j < height; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = height + 1; j < height + 2 + counter; j++)
        {
            printf("#");
        }
        // counter update and next line
        counter++;
        printf("\n");
    }
}