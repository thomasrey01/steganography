
#ifndef IMAGE_H
#define IMAGE_H

/*
 Structs for bmp file formats. See https://en.wikipedia.org/wiki/BMP_file_format 
 for more info.
*/

struct BITMAP_header {
    char name[2];
    unsigned int size;  
    int garbage;
    unsigned int imageOffset;
};

struct DIB_header {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short int colorPlanes;
    unsigned short int bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int temp[4];
};

struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Image {
    int height;
    int width;
    struct RGB **rgb;
};

struct Data{
    struct BITMAP_header header;
    struct DIB_header dibheader;
    struct Image image;
};

struct Image readImage(FILE *fp, int height, int width);
void printRGB(struct Image image);
void freeImage(struct Image pic);
struct Data openBMPFile(char *filename);
void createNewImage(struct BITMAP_header header, struct DIB_header dibheader, struct RGB **rgb, struct Image pic);

#endif