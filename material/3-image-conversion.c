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

    FILE *redFile = fopen("red.pgm", "wb");
    FILE *greenFile = fopen("green.pgm", "wb");
    FILE *blueFile = fopen("blue.pgm", "wb");

    if (redFile == NULL || greenFile == NULL || blueFile == NULL)
    {
        printf("Error creating output files.\n");
        free(imageData);
        return 1;
    }

        fprintf(redFile, "P5\n%d %d\n%d\n", width, height, maxval);
    fprintf(greenFile, "P5\n%d %d\n%d\n", width, height, maxval);
    fprintf(blueFile, "P5\n%d %d\n%d\n", width, height, maxval);

    unsigned char *redComponent = (unsigned char *)malloc(width * height);
    unsigned char *greenComponent = (unsigned char *)malloc(width * height);
    unsigned char *blueComponent = (unsigned char *)malloc(width * height);

    if (redComponent == NULL || greenComponent == NULL || blueComponent == NULL)
    {
        printf("Memory allocation failed.\n");
        free(imageData);
        fclose(redFile);
        fclose(greenFile);
        fclose(blueFile);
        return 1;
    }

    for (int i = 0; i < width * height; i++)
    {
        redComponent[i] = imageData[3 * i];       // Red
        greenComponent[i] = imageData[3 * i + 1]; // Green
        blueComponent[i] = imageData[3 * i + 2];  // Blue
    }

    fwrite(redComponent, 1, width * height, redFile);
    fwrite(greenComponent, 1, width * height, greenFile);
    fwrite(blueComponent, 1, width * height, blueFile);

    free(imageData);
    free(redComponent);
    free(greenComponent);
    free(blueComponent);
    fclose(redFile);
    fclose(greenFile);
    fclose(blueFile);

    printf("Red, Green, and Blue component images saved as red.pgm, green.pgm, and blue.pgm.\n");

    return 0;
}
