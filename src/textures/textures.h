#ifndef textures_h
#define textures_h

#define GL_SILENCE_DEPRECATION

#include <GL/glut.h>
#include <iostream>
#include <fstream>

typedef unsigned char uchar_t;

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetData;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

extern GLuint gtextureID;

uchar_t* loadImage(const char* filename, int &width, int &height);
GLuint loadTexture(const char* filename);
void drawGrass();


#endif
