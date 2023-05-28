#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //get_string collects input from the user, that then stores it in variable string called name
    string name = get_string("What's your name? ");

    //prints the result in the terminal
    printf("hello, %s\n", name);
}