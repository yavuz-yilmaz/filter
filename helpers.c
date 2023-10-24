#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;
            BYTE avg = round((red + green + blue) / (float) 3);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int add_counter = width - 1;

    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            if (add_counter > 0)
            {
                RGBTRIPLE temp = image[i][j + add_counter];
                image[i][j + add_counter] = image[i][j];
                image[i][j] = temp;
                add_counter -= 2;
            }
        }
        add_counter = width - 1;
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, red, green, blue, counter; j < width; j++)
        {
            red = 0, green = 0, blue = 0, counter = 0;

            for (int h = i - 1; h < i + 2; h++)
            {
                for (int w = j - 1; w < j + 2; w++)
                {
                    if ((h >= 0 && h < height) && (w >= 0 && w < width))
                    {
                        red += temp[h][w].rgbtRed;
                        green += temp[h][w].rgbtGreen;
                        blue += temp[h][w].rgbtBlue;
                        counter++;
                    }
                }
            }

            image[i][j].rgbtRed = round(red / (float) counter);
            image[i][j].rgbtGreen = round(green / (float) counter);
            image[i][j].rgbtBlue = round(blue / (float) counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // For each pixel
            int rowCoords[] = {i - 1, i, i + 1};
            int colCoords[] = {j - 1, j, j + 1};
            float GxRed = 0, GxGreen = 0, GxBlue = 0;
            float GyRed = 0, GyGreen = 0, GyBlue = 0;
            
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int curRow = rowCoords[r];
                    int curCol = colCoords[c];

                    if ((curRow >= 0 && curRow < height) && (curCol >= 0 && curCol < width))
                    {
                        RGBTRIPLE pixel = image[curRow][curCol];

                        GxRed += Gx[r][c] * pixel.rgbtRed;
                        GxGreen += Gx[r][c] * pixel.rgbtGreen;
                        GxBlue += Gx[r][c] * pixel.rgbtBlue;

                        GyRed += Gy[r][c] * pixel.rgbtRed;
                        GyGreen += Gy[r][c] * pixel.rgbtGreen;
                        GyBlue += Gy[r][c] * pixel.rgbtBlue;
                    }
                }
            }

            int finalRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int finalGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int finalBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            temp[i][j].rgbtRed = finalRed > 255 ? 255 : finalRed;
            temp[i][j].rgbtGreen = finalGreen > 255 ? 255 : finalGreen;
            temp[i][j].rgbtBlue = finalBlue > 255 ? 255 : finalBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}