#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "image.h"


/*
 Checks if string can fit into image.
*/
bool canFit(uint size, uint numChar, uint dimension)
{
    uint num = size + (numChar * 3);
    return (num <= dimension * 4);
}

/*
 Returns total ASCII size of string
*/
unsigned int getSize(char *str)
{
    char *strCpy = str;
    unsigned int size = 0;
    while (*strCpy != '\0') {
        size += (int)*strCpy;
        strCpy = strCpy + 1;
    }
    return size;
}

/*
 Changes the values of blue in the pixel array of the bmp file in groups of 4 values
 per pixel, as to not make the changes too noticeable.
*/
void encrypt(char *filename)
{
    printf("Type in your message (max 1000 characters): ");
    char message[1000];
    scanf("%[^\n]%*c", message);

    struct Data data = openBMPFile(filename);
    struct RGB **rgb = data.image.rgb;
    int width = data.image.width;
    int height = data.image.height;
    int h, w, c, shift;

    if (canFit(getSize(message), strlen(message), height * width)) {
        int len = -1;
        int idx = 0;
        while (message[idx] != '\0') {
            c = (int)message[idx];
            while(c > 0) {
                len++;
                h = len / width;
                w = len % width;
                shift = (c > 4 ? 4 : c);
                rgb[h][w].blue = (rgb[h][w].blue + shift <= 255 ? rgb[h][w].blue + shift : rgb[h][w].blue - shift);
                c -= shift;
            }
            len++;
            idx++;
        }
        createNewImage(data.header, data.dibheader, rgb, data.image);
    } else {
        printf("Error: the image is too small for this message\n");
        freeImage(data.image);
        exit(-1);
    }
    freeImage(data.image);
}

/*
 Extracts hidden message by comparing the blue values of the RGB pixel array 
 from the two images. NOTE: the two images need to have the same dimensions or else 
 return statement is executed.
*/
void decrypt(char *image1, char *image2)
{
    int numSpaces = 0;
    struct Data data1 = openBMPFile(image1);
    struct Data data2 = openBMPFile(image2);

    if (data1.dibheader.height * data1.dibheader.width != data2.dibheader.height * data2.dibheader.width) {
        printf("incompatible images\n");
        return;
    }
    struct RGB **rgb1 = data1.image.rgb;
    struct RGB **rgb2 = data2.image.rgb;
    uint len = 0, w, h;
    int width = data1.dibheader.width, height = data1.dibheader.height;
    uint dimension = data1.dibheader.height * data1.dibheader.width;
    int dif, idx = 0;
    int c = 0;
    char message[1000];
    while (numSpaces < 3 && len < dimension && idx < 1000) {
        w = len % width;
        h = len / width;
        dif = abs(rgb1[h][w].blue - rgb2[h][w].blue);
        c += dif;
        if (dif == 0) {
            numSpaces++;
            if (c != 0) {
                message[idx] = c;
                idx++;
            }
            c = 0;
        } else {
            numSpaces = 0;
        }
        len++;
    }
    message[idx] = '\0';
    printf("%s\n", message);
    freeImage(data1.image);
    freeImage(data2.image);
}


int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "encrypt") == 0) {
            encrypt(argv[2]);
        } else if (strcmp(argv[1], "decrypt") == 0) {
            decrypt(argv[2], argv[3]);
        } else {
            printf("Invalid arguments\n");
        }
    } else {
        printf("Invalid arguments\n");
    }
}