#pragma once
#include <string>
#include <vector>
using namespace std;

struct Header{
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct Pixel{
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
    float RedNormal;
    float GreenNormal;
    float BlueNormal;
    Pixel(){}
    Pixel(unsigned char Blue_, unsigned char Green_, unsigned char Red_);
};

struct ImageData{
    string filename;
    Header header;
    vector<Pixel> ImagePixels;
    ImageData(){}
    ImageData(string filename_, Header &header_, vector<Pixel>& ImagePixels_);
};

