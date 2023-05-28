#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long number;
    int sum_a = 0;
    int sum_b = 0;
    int sum_total = 0;
    int holder;
    int extra;
    int counter = 0;
    long id = 0;
    int invalid = 0;
    do
    {
        number = get_long("Number: ");
        id = number;
    }
    while (number < 0);

    while (number > 0)
    {
        // Register the digit
        int digit = number % 10;
        // Removes the registered digit from the number
        number = (number - digit) / 10;
        // Loop until 0 or number
        counter++;
        // odd digits
        if (counter % 2 != 0)
        {
            sum_b = sum_b + digit;
        }
        // even digits
        else
        {
            holder = digit * 2;
            // check if is more than one digit
            if (holder < 10)
            {
                sum_a = sum_a + holder;
            }
            else
            {
                extra = holder % 10;
                holder = (holder - extra) / 10;
                sum_a = sum_a + extra + holder;
            }
        }
    }
    sum_total = sum_a + sum_b;
    // reestablishing number
    number = id;
    if ((sum_total % 10 == 0) && (counter < 17) && (counter > 12))
    {
        // Get the first two digits
        for (int i = 0; i < counter - 2; i++)
        {
            extra = number % 10;
            number = (number - extra) / 10;
        }
        // Check for VISA number
        if ((number < 50) && (number > 39))
        {
            printf("VISA\n");
        }
        else
        {
            invalid = invalid + 1;
        }
        // Check for Mastercard
        if (((number < 56) && (number > 50)) && (counter == 16))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            invalid = invalid + 1;
        }
        // Check for American Express
        if (((number == 34) || (number == 37)) && (counter == 15))
        {
            printf("AMEX\n");
        }
        else
        {
            invalid = invalid + 1;
        }
    }
    else
    {
        printf("INVALID\n");
    }
    if (invalid == 3)
    {
        printf("INVALID\n");
    }
}