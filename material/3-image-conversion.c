#include <stdio.h>
#include <stdlib.h>

// Function to skip comments and whitespace in PPM file
void skipCommentsAndWhitespace(FILE *file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '#') { // Skip comments (lines starting with '#')
            while (fgetc(file) != '\n');
        } else if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t') {
            ungetc(ch, file); // Put back the first non-whitespace character
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_ppm_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    // Reading the PPM header
    char format[3];
    int width, height, maxval;
    fscanf(inputFile, "%2s", format);

    // Check if it's a binary PPM (P6)
    if (format[0] != 'P' || format[1] != '6') {
        printf("Input file is not a binary PPM (P6) image.\n");
        fclose(inputFile);
        return 1;
    }

    // Skip comments and whitespace, and read width, height, and maxval
    skipCommentsAndWhitespace(inputFile);
    fscanf(inputFile, "%d %d", &width, &height);
    skipCommentsAndWhitespace(inputFile);
    fscanf(inputFile, "%d", &maxval);
    fgetc(inputFile); // Skip the single whitespace character after maxval

    // Allocate memory to store the pixel data (RGB)
    unsigned char *imageData = (unsigned char *)malloc(3 * width * height);
    if (imageData == NULL) {
        printf("Memory allocation failed.\n");
        fclose(inputFile);
        return 1;
    }

    // Read the pixel data from the PPM file
    fread(imageData, 3, width * height, inputFile);
    fclose(inputFile);

    // Open output files for Red, Green, and Blue components
    FILE *redFile = fopen("red.pgm", "wb");
    FILE *greenFile = fopen("green.pgm", "wb");
    FILE *blueFile = fopen("blue.pgm", "wb");

    if (redFile == NULL || greenFile == NULL || blueFile == NULL) {
        printf("Error creating output files.\n");
        free(imageData);
        return 1;
    }

    // Write PGM headers to the three files
    fprintf(redFile, "P5\n%d %d\n%d\n", width, height, maxval);
    fprintf(greenFile, "P5\n%d %d\n%d\n", width, height, maxval);
    fprintf(blueFile, "P5\n%d %d\n%d\n", width, height, maxval);

    // Allocate memory for the grayscale images
    unsigned char *redComponent = (unsigned char *)malloc(width * height);
    unsigned char *greenComponent = (unsigned char *)malloc(width * height);
    unsigned char *blueComponent = (unsigned char *)malloc(width * height);

    if (redComponent == NULL || greenComponent == NULL || blueComponent == NULL) {
        printf("Memory allocation failed.\n");
        free(imageData);
        fclose(redFile);
        fclose(greenFile);
        fclose(blueFile);
        return 1;
    }

    // Split the RGB components into three grayscale images
    for (int i = 0; i < width * height; i++) {
        redComponent[i] = imageData[3 * i];      // Red component
        greenComponent[i] = imageData[3 * i + 1]; // Green component
        blueComponent[i] = imageData[3 * i + 2];  // Blue component
    }

    // Write grayscale images to their respective files
    fwrite(redComponent, 1, width * height, redFile);
    fwrite(greenComponent, 1, width * height, greenFile);
    fwrite(blueComponent, 1, width * height, blueFile);

    // Free memory and close files
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
