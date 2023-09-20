#include "helpers.h"
#include <math.h> // for sqrt() and round()
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float total = image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed;
            float avg = round(total / 3);
            image[h][w].rgbtBlue = (BYTE) avg;
            image[h][w].rgbtGreen = (BYTE) avg;
            image[h][w].rgbtRed = (BYTE) avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < (int) width / 2; w++)
        {
            RGBTRIPLE *old_addy = &image[h][w];
            RGBTRIPLE *new_addy = &image[h][width-w-1];
            RGBTRIPLE buffer = *new_addy;
            *new_addy = *old_addy;
            *old_addy = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

// make a copy to reference
    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

// do the thing
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        { // all averages include the pixel itself
            int sumBlue = (int) copy[h][w].rgbtBlue;
            int sumGreen = (int) copy[h][w].rgbtGreen;
            int sumRed = (int) copy[h][w].rgbtRed;
            int numPixels = 1;

        // non-diagonal pixels
            if (h != 0)
            { // not first row, add pixel above
                sumBlue += copy[h-1][w].rgbtBlue;
                sumGreen += copy[h-1][w].rgbtGreen;
                sumRed += copy[h-1][w].rgbtRed;
                numPixels += 1;
            }
            if (h != height - 1)
            { // not last row, add pixel below
                sumBlue += copy[h+1][w].rgbtBlue;
                sumGreen += copy[h+1][w].rgbtGreen;
                sumRed += copy[h+1][w].rgbtRed;
                numPixels += 1;
            }
            if (w != 0)
            { // not first column, add pixel to left
                sumBlue += copy[h][w-1].rgbtBlue;
                sumGreen += copy[h][w-1].rgbtGreen;
                sumRed += copy[h][w-1].rgbtRed;
                numPixels += 1;
            }
            if (w != width - 1)
            { // not last column, add pixel to right
                sumBlue += copy[h][w+1].rgbtBlue;
                sumGreen += copy[h][w+1].rgbtGreen;
                sumRed += copy[h][w+1].rgbtRed;
                numPixels += 1;
            }

        // diagonal pixels
            if (h != 0 && w != 0)
            { // not first row or column, add upper left pixel
                sumBlue += copy[h-1][w-1].rgbtBlue;
                sumGreen += copy[h-1][w-1].rgbtGreen;
                sumRed += copy[h-1][w-1].rgbtRed;
                numPixels += 1;
            }
            if (h != 0 && w != width - 1)
            { // not first row or last column, add upper right pixel
                sumBlue += copy[h-1][w+1].rgbtBlue;
                sumGreen += copy[h-1][w+1].rgbtGreen;
                sumRed += copy[h-1][w+1].rgbtRed;
                numPixels += 1;
            }
            if (h != height - 1 && w != 0)
            { // not last row or first column, add lower left pixel
                sumBlue += copy[h+1][w-1].rgbtBlue;
                sumGreen += copy[h+1][w-1].rgbtGreen;
                sumRed += copy[h+1][w-1].rgbtRed;
                numPixels += 1;
            }
            if (h != height - 1 && w != width -1)
            { // not last row or last column, add lower right pixel
                sumBlue += copy[h+1][w+1].rgbtBlue;
                sumGreen += copy[h+1][w+1].rgbtGreen;
                sumRed += copy[h+1][w+1].rgbtRed;
                numPixels += 1;
            }
            image[h][w].rgbtBlue = (BYTE) round((float)sumBlue / (float)numPixels);
            image[h][w].rgbtGreen = (BYTE) round((float)sumGreen / (float)numPixels);
            image[h][w].rgbtRed = (BYTE) round((float)sumRed / (float)numPixels);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
// make a copy to reference
    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

// do the thing
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int GxB = 0;
            int GyB = 0;
            int GxG = 0;
            int GyG = 0;
            int GxR = 0;
            int GyR = 0;

        // non-diagonal pixels
            if (h != 0)
            { // not first row, add pixel above
                GyB -= 2 * (int) copy[h-1][w].rgbtBlue;
                GyG -= 2 * (int) copy[h-1][w].rgbtGreen;
                GyR -= 2 * (int) copy[h-1][w].rgbtRed;
            }
            if (h != height - 1)
            { // not last row, add pixel below
                GyB += 2 * (int) copy[h+1][w].rgbtBlue;
                GyG += 2 * (int) copy[h+1][w].rgbtGreen;
                GyR += 2 * (int) copy[h+1][w].rgbtRed;
            }
            if (w != 0)
            { // not first column, add pixel to left
                GxB -= 2 * (int) copy[h][w-1].rgbtBlue;
                GxG -= 2 * (int) copy[h][w-1].rgbtGreen;
                GxR -= 2 * (int) copy[h][w-1].rgbtRed;
            }
            if (w != width - 1)
            { // not last column, add pixel to right
                GxB += 2 * (int) copy[h][w+1].rgbtBlue;
                GxG += 2 * (int) copy[h][w+1].rgbtGreen;
                GxR += 2 * (int) copy[h][w+1].rgbtRed;
            }

        // diagonal pixels
            if (h != 0 && w != 0)
            { // upper left pixel
                GxB -= (int) copy[h-1][w-1].rgbtBlue;
                GxG -= (int) copy[h-1][w-1].rgbtGreen;
                GxR -= (int) copy[h-1][w-1].rgbtRed;

                GyB -= (int) copy[h-1][w-1].rgbtBlue;
                GyG -= (int) copy[h-1][w-1].rgbtGreen;
                GyR -= (int) copy[h-1][w-1].rgbtRed;
            }
            if (h != 0 && w != width - 1)
            { // upper right pixel
                GxB += (int) copy[h-1][w+1].rgbtBlue;
                GxG += (int) copy[h-1][w+1].rgbtGreen;
                GxR += (int) copy[h-1][w+1].rgbtRed;

                GyB -= (int) copy[h-1][w+1].rgbtBlue;
                GyG -= (int) copy[h-1][w+1].rgbtGreen;
                GyR -= (int) copy[h-1][w+1].rgbtRed;
            }
            if (h != height - 1 && w != 0)
            { // lower left pixel
                GxB -= (int) copy[h+1][w-1].rgbtBlue;
                GxG -= (int) copy[h+1][w-1].rgbtGreen;
                GxR -= (int) copy[h+1][w-1].rgbtRed;

                GyB += (int) copy[h+1][w-1].rgbtBlue;
                GyG += (int) copy[h+1][w-1].rgbtGreen;
                GyR += (int) copy[h+1][w-1].rgbtRed;
            }
            if (h != height - 1 && w != width -1)
            { // lower right pixel
                GxB += (int) copy[h+1][w+1].rgbtBlue;
                GxG += (int) copy[h+1][w+1].rgbtGreen;
                GxR += (int) copy[h+1][w+1].rgbtRed;

                GyB += (int) copy[h+1][w+1].rgbtBlue;
                GyG += (int) copy[h+1][w+1].rgbtGreen;
                GyR += (int) copy[h+1][w+1].rgbtRed;
            }

            double underB = GxB*GxB + GyB*GyB;
            double underG = GxG*GxG + GyG*GyG;
            double underR = GxR*GxR + GyR*GyR;

            double finalB = round(sqrt(underB));
            double finalG = round(sqrt(underG));
            double finalR = round(sqrt(underR));

            if (finalB > 255)
                finalB = 255;
            if (finalG > 255)
                finalG = 255;
            if (finalR > 255)
                finalR = 255;

            image[h][w].rgbtBlue = (BYTE) finalB;
            image[h][w].rgbtGreen = (BYTE) finalG;
            image[h][w].rgbtRed = (BYTE) finalR;
        }
    }
    return;
}