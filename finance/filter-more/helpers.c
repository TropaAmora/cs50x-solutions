#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}
// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = 0;
    // check to see if the height is an odd or even integer
    if (width % 2 == 0)
    {
        half = width / 2;
    }
    else
    {
        half = (width - 1) / 2;
    }
    // runs through half the image and exchanges the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            RGBTRIPLE temp[height][width]; //why? ro is answering...
            temp[i][j] = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp[i][j];
        }
    }
    return;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // loops through the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sum_green = 0;
            float sum_red = 0;
            float sum_blue = 0;
            float counter = 0;

            // runs through the 3x3 array of neighbouring values
            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    // checks if the pixel is inside of the picture
                    if (i + a < 0 || i + a > height - 1)
                    {
                        continue;
                    }
                    // checks if the pixel is inside of the picture
                    if (j + b < 0 || j + b > width - 1)
                    {
                        continue;
                    }
                    // checks if the current pixel present in the array "inside" of the picture
                    sum_green += image[i + a][j + b].rgbtGreen;
                    sum_red += image[i + a][j + b].rgbtRed;
                    sum_blue += image[i + a][j + b].rgbtBlue;
                    counter++;
                }
            }
            temp[i][j].rgbtGreen = round(sum_green / counter);
            temp[i][j].rgbtRed = round(sum_red / counter);
            temp[i][j].rgbtBlue = round(sum_blue / counter);
        }
    }
    // run through the image again to update the values with the new ones
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int g_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int g_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // runs through the picture
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gx_green = 0;
            float gx_red = 0;
            float gx_blue = 0;
            float gy_green = 0;
            float gy_red = 0;
            float gy_blue = 0;
            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    // checks if the current pixel present in the array is "inside" of the picture
                    if (i + a < 0 || i + a > height - 1)
                    {
                        continue;
                    }
                    if (j + b < 0 || j + b > width - 1)
                    {
                        continue;
                    }

                    // checks if the current pixel present in the array is "inside" of the picture
                    gx_green += image[i + a][j + b].rgbtGreen * g_x[a + 1][b + 1];
                    gx_red += image[i + a][j + b].rgbtRed * g_x[a + 1][b + 1];
                    gx_blue += image[i + a][j + b].rgbtBlue * g_x[a + 1][b + 1];
                    gy_green += image[i + a][j + b].rgbtGreen * g_y[a + 1][b + 1];
                    gy_red += image[i + a][j + b].rgbtRed * g_y[a + 1][b + 1];
                    gy_blue += image[i + a][j + b].rgbtBlue * g_y[a + 1][b + 1];
                }
            }
            // do the meth
            int green = round(sqrt((gx_green * gx_green) + (gy_green * gy_green)));
            int red = round(sqrt((gx_red * gx_red) + (gy_red * gy_red)));
            int blue = round(sqrt((gx_blue * gx_blue) + (gy_blue * gy_blue)));

            temp[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
            temp[i][j].rgbtGreen = (green > 255) ? 255 : green;
            temp[i][j].rgbtRed = (red > 255) ? 255 : red;
        }
    }
    // run through the image again to update the values with the new ones
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
