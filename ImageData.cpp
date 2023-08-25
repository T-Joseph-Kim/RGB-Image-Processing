#include "ImageData.h"

Pixel::Pixel(unsigned char Red_, unsigned char Green_, unsigned char Blue_) {
    Red = Red_;
    Green = Green_;
    Blue = Blue_;
    RedNormal = (float)Red_/255;
    GreenNormal = (float)Green_/255;
    BlueNormal = (float)Blue_/255;
}

ImageData::ImageData(string filename_, Header &header_, vector<Pixel> &ImagePixels_) {
    filename = filename_;
    header = header_;
    ImagePixels = ImagePixels_;
}


