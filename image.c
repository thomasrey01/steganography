#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

/*
 Returns Image struct from file
*/
struct Image readImage(FILE *fp, int height, int width) 
{
    struct Image pic;
    pic.rgb = (struct RGB**)malloc(height * sizeof(void*));
    pic.height = height;
    pic.width = width;
    for (int i = height - 1; i >= 0; i--) {
        pic.rgb[i] = (struct RGB*)malloc(width * sizeof(struct RGB));
        fread(pic.rgb[i], width, sizeof(struct RGB), fp);
    }
    return pic;
}

/*
 Takes an RGB pixelarray and old image formated headers as parameters
 and creates a new image file
*/
void createNewImage(struct BITMAP_header header, struct DIB_header dibheader, struct RGB **rgb, struct Image pic)
{
    FILE *fpw = fopen("imageEncrypted.bmp", "w");
    if (fpw == NULL) {
        printf("error\n");
        exit(-1);
    }
    fwrite(header.name, 2, 1, fpw);
    fwrite(&header.size, 3 * sizeof(int), 1, fpw);
    fwrite(&dibheader, sizeof(struct DIB_header), 1, fpw);

    fwrite(pic.rgb[pic.height-1], pic.width, sizeof(struct RGB), fpw);
    for (int i = pic.height - 2; i >= 0; i--) 
        fwrite(rgb[i], pic.width, sizeof(struct RGB), fpw);
    fclose(fpw);
}

/*
 Prints the rgb pixel array
*/
void printRGB(struct Image image) 
{
    for (int i = image.height - 1; i >= 0; i--) {
        for (int j = 0; j < image.width; j++) {
            printf("%d %d %d: ", image.rgb[i][j].blue, image.rgb[i][j].green, image.rgb[i][j].red);
        }
        printf("\n");
    }
}

/* Frees the image
*/
void freeImage(struct Image pic) 
{
    for (int i = 0; i < pic.height; i++) {
        free(pic.rgb[i]);
    }
    free(pic.rgb);
}


struct Data openBMPFile(char *filename) 
{
    FILE *fp = fopen(filename, "r");
    struct Data data;

    if (fp == NULL) {
        printf("file null");
        exit(-1);
    }

    struct BITMAP_header header;
    struct DIB_header dibheader;
    fread(header.name, 2, 1, fp);
    fread(&header.size, 3*sizeof(unsigned int), 1, fp);

    fread(&dibheader.headerSize, sizeof(struct DIB_header), 1, fp);

    data.header = header;
    data.dibheader = dibheader;

    if((dibheader.headerSize != 40) || (dibheader.compression != 0) //If image is compressed then this doesn't work
        || (dibheader.bitsPerPixel != 24)) {
        printf("No good...\n");
        fclose(fp);
        exit(-1);
    }

    fseek(fp, header.imageOffset, SEEK_SET);
    struct Image image = readImage(fp, dibheader.height, dibheader.width);
    data.image = image;
    fclose(fp);
    return data;
}