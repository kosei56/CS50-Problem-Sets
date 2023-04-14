#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = (int)round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue)/3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tempImage[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            tempImage[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            tempImage[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tempImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = tempImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tempImage[i][j].rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;
            int count = 0;
            for (int r = i - 1; r < i + 2; r++)
            {
                for(int c = j - 1; c < j + 2; c++)
                {
                    if(r >= 0 && r < height && c >= 0 && c < width )
                    {
                    totalRed += image[r][c].rgbtRed;
                    totalGreen += image[r][c].rgbtGreen;
                    totalBlue += image[r][c].rgbtBlue;
                    count++;
                    }

                }
            }
            tempImage[i][j].rgbtRed = (int)round(totalRed / (double)count);
            tempImage[i][j].rgbtGreen = (int)round(totalGreen / (double)count);
            tempImage[i][j].rgbtBlue = (int)round(totalBlue / (double)count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tempImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = tempImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tempImage[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = {
        {-1, -2, -1},
        { 0, 0, 0},
        { 1, 2, 1}
    };
    int initR;
    int initC;

    for (int i = 0; i < height; i++)
    {
        initR = i - 1;
        for (int j = 0; j < width; j++)
        {
            int newRedX = 0;
            int newGreenX = 0;
            int newBlueX = 0;
            int newRedY = 0;
            int newGreenY = 0;
            int newBlueY = 0;
            initC = j - 1;
            for (int r = i - 1; r < i + 2; r++)
            {
                for(int c = j - 1; c < j + 2; c++)
                {
                    if(r >= 0 && r < height && c >= 0 && c < width )
                    {
                        newRedX += image[r][c].rgbtRed * Gx[r - initR][c - initC];
                        newGreenX += image[r][c].rgbtGreen * Gx[r - initR][c - initC];
                        newBlueX += image[r][c].rgbtBlue * Gx[r - initR][c - initC];

                        newRedY += image[r][c].rgbtRed * Gy[r - initR][c - initC];
                        newGreenY += image[r][c].rgbtGreen * Gy[r - initR][c - initC];
                        newBlueY += image[r][c].rgbtBlue * Gy[r - initR][c - initC];
                    }
                }
            }
            int newRed;
            int newGreen;
            int newBlue;
            newRed = (int)round(sqrt(newRedX * newRedX + newRedY * newRedY));
            newGreen = (int)round(sqrt(newGreenX * newGreenX + newGreenY * newGreenY));
            newBlue = (int)round(sqrt(newBlueX * newBlueX + newBlueY * newBlueY));

            if (newRed > 255)
            {
                newRed = 255;
            }
            if (newGreen > 255)
            {
                newGreen = 255;
            }
            if (newBlue > 255)
            {
                newBlue = 255;
            }
            tempImage[i][j].rgbtRed = newRed;
            tempImage[i][j].rgbtGreen = newGreen;
            tempImage[i][j].rgbtBlue = newBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tempImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = tempImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tempImage[i][j].rgbtBlue;
        }
    }
    return;
}
