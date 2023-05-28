#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // first step is to check for the correct command line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE.\n");
        return 1;
    }
    // open card.raw file
    char *input_file_name = argv[1];
    FILE *input_pointer = fopen(input_file_name, "r");
    if (input_pointer == NULL)
    {
        printf("Error: cannot open %s\n", input_file_name);
        return 2;
    }
    // size of the chunks of data used in jpeg
    int BLOCK_SIZE = 512;

    // array of 512 elements to store the 512 bytes from the raw file
    BYTE buffer[BLOCK_SIZE];
    int counter = 0;

    // set output file pointer to NULL
    FILE *image_pointer = NULL;

    // create an array (or string) to store the name of the file
    char filename[8] = {0};

    // loop through the FILE until the end of it
    while (fread(&buffer, BLOCK_SIZE, 1, input_pointer) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if it is not the first one, close the previous
            if (image_pointer != NULL)
            {
                fclose(image_pointer);
            }

            // start output file
            sprintf(filename, "%03i.jpg", counter);
            image_pointer = fopen(filename, "w");
            counter++;
        }

        // if successful, write to file
        if (image_pointer != NULL)
        {
            fwrite(&buffer, sizeof(BYTE) * BLOCK_SIZE, 1, image_pointer);
        }
    }
    // closing the last opened output
    if (image_pointer != NULL)
    {
        fclose(image_pointer);
    }

    // close the input FILE, as it is jnot needed anymore
    fclose(input_pointer);

    return 0;
}