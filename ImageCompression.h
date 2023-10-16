#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<math.h>

// bmfh = Bit Map File Header
typedef struct BMPFileheader{

    unsigned char bfType1; // for BMP type1='B' 
    unsigned char bfType2; // type2='M'
    unsigned char bfSize[4]; // size of file in bytes
    unsigned short bfReserved1; // must set to 0
    unsigned short bfReserved2; // must set to 0
    unsigned char bfOffset[4]; // offset of start of image data.

}BMPFileheader;

// bmih = Bit Map Info Header
typedef struct BMPInfoHeader{

    unsigned char headerLenth[4]; // specifies the bitmap size.
    unsigned char width[4]; // Width of Image.
    unsigned char height[4]; // Height of Image.
    
}BMPInfoHeader;

typedef struct SubSection{

    int tlx; // Top Left x - coordinate of Sub Section.
    int tly; // Top Left y - coordinate of Sub Section.
    int w;  // Width of Sub Section.
    int h;  // Height of Sub Section.
    unsigned char avg; // average intensity in Sub Section
    double mse; // MSE = Mean Squarred Error 
    //  mse = (average of sum of squares) - (avg * avg).

}SubSection;

typedef struct childSections{

    SubSection *ChildNW; // North-West Quadrant 
    SubSection *ChildNE; // North-East Quadrant
    SubSection *ChildSW; // South-West Quadrant
    SubSection *ChildSE; // South-East Quadrant

}childSections;

typedef struct QuadNode{

    SubSection *Image;

    struct QuadNode* ChildNW; // North-West Child
    struct QuadNode* ChildNE; // North-East Child
    struct QuadNode* ChildSW; // South-West Child
    struct QuadNode* ChildSE; // South-East Child

    struct QuadNode *parent;


}QuadNode;

void imageCompression(char * filename);