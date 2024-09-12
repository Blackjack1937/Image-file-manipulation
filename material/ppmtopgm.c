#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

int pm_getint(FILE *file);
unsigned char pm_getrawbyte(FILE *file);

int main(int argc, char *argv[])
{
    if (argc != 2) // added one parameter to the args count (no need actually)
    {
        printf("Usage: %s <input_ppm_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL)
    {
        printf("Error opening input file.\n");
        return 1;
    }

    int brightness_adjust = scanf("Enter a brightness adjustment value : ");

    char format[3];
    format[0] = pm_getrawbyte(inputFile);
    format[1] = pm_getrawbyte(inputFile);
    format[2] = '\0';

    if (format[0] != 'P' || format[1] != '6')
    {
        printf("Input file is not a binary PPM (P6) image.\n");
        fclose(inputFile);
        return 1;
    }

    int width = pm_getint(inputFile);
    int height = pm_getint(inputFile);
    int maxval = pm_getint(inputFile);

    if (maxval != 255)
    {
        printf("Unsupported max color value (only 255 is supported).\n");
        fclose(inputFile);
        return 1;
    }

    unsigned char *imageData = (unsigned char *)malloc(3 * width * height);
    if (imageData == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(inputFile);
        return 1;
    }

    fread(imageData, 3, width * height, inputFile);
    fclose(inputFile);

    FILE *outputFile = fopen("output.pgm", "wb");
    if (outputFile == NULL)
    {
        printf("Error creating output file.\n");
        free(imageData);
        return 1;
    }

    // Write the PGM header
    fprintf(outputFile, "P5\n%d %d\n%d\n", width, height, maxval);

    // Allocate memory for the grayscale image
    unsigned char *grayImage = (unsigned char *)malloc(width * height);
    if (grayImage == NULL)
    {
        printf("Memory allocation failed.\n");
        free(imageData);
        fclose(outputFile);
        return 1;
    }

    // Convert RGB to grayscale
    for (int i = 0; i < width * height; i++)
    {
        unsigned char r = imageData[3 * i];     // Red component
        unsigned char g = imageData[3 * i + 1]; // Green component
        unsigned char b = imageData[3 * i + 2]; // Blue component

        // Calculate grayscale value using weighted sum of RGB components
        unsigned char rgb = (r + g + b) / 3 - brightness_adjust;

        if (rgb < 0 || rgb > 255)
        {
            grayImage[i] = (unsigned char)((r + g + b) / 3 - brightness_adjust);
        }
        else
        {
            printf("The brightness adjustment is invalid");
            return 1;
        }
    }

    // Write the grayscale image data to the PGM file
    fwrite(grayImage, 1, width * height, outputFile);

    // Free allocated memory and close files
    free(imageData);
    free(grayImage);
    fclose(outputFile);

    printf("PPM image successfully converted to output.pgm.\n");

    return 0;
}
