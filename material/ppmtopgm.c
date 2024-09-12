#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

int pm_getint(FILE *file);
unsigned char pm_getrawbyte(FILE *file);

int main(int argc, char *argv[])
{
    if (argc != 2)
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

    // Getting the brightness adjustment parameter from stdin
    int brightness_adjust;
    printf("Enter a brightness adjustment value: ");
    scanf("%d", &brightness_adjust);

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
        unsigned char r = imageData[3 * i];     // Red value
        unsigned char g = imageData[3 * i + 1]; // Green value
        unsigned char b = imageData[3 * i + 2]; // Blue value

        int grayscale_value = (r + g + b) / 3;

        grayscale_value += brightness_adjust;

        if (grayscale_value > 255)
        {
            grayscale_value = 255;
        }
        else if (grayscale_value < 0)
        {
            grayscale_value = 0;
        }

        grayImage[i] = (unsigned char)grayscale_value;
    }

    fwrite(grayImage, 1, width * height, outputFile);

    free(imageData);
    free(grayImage);
    fclose(outputFile);

    printf("PPM image successfully converted to output.pgm.\n");

    return 0;
}
