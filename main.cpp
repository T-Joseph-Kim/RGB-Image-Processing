#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include "ImageData.h"
using namespace std;

ImageData OpenAndStore(const string& FileName){
    ifstream inFile(FileName, ios_base::binary);
    ImageData image;
    image.filename = FileName;
    if (inFile.is_open()){
        inFile.read((char*)(&image.header.idLength), 1);
        inFile.read((char*)(&image.header.colorMapType), 1);
        inFile.read((char*)(&image.header.dataTypeCode), 1);
        inFile.read((char*)(&image.header.colorMapOrigin), 2);
        inFile.read((char*)(&image.header.colorMapLength), 2);
        inFile.read((char*)(&image.header.colorMapDepth), 1);
        inFile.read((char*)(&image.header.xOrigin), 2);
        inFile.read((char*)(&image.header.yOrigin), 2);
        inFile.read((char*)(&image.header.width), 2);
        inFile.read((char*)(&image.header.height), 2);
        inFile.read((char*)(&image.header.bitsPerPixel), 1);
        inFile.read((char*)(&image.header.imageDescriptor), 1);
        int image_size = image.header.width * image.header.height;
        for (unsigned int i = 0; i < image_size; ++i){
            Pixel NewPixel;
            inFile.read((char*)(&NewPixel.Blue), 1);
            inFile.read((char*)(&NewPixel.Green), 1);
            inFile.read((char*)(&NewPixel.Red), 1);
            NewPixel.BlueNormal = (float)NewPixel.Blue/255;
            NewPixel.GreenNormal = (float)NewPixel.Green/255;
            NewPixel.RedNormal = (float)NewPixel.Red/255;
            image.ImagePixels.push_back(NewPixel);
        }
    }
    inFile.close();
    return image;
}
void WriteFile(const ImageData& image, const string& FileName){
    ofstream outFile(FileName, ios::out | ios::binary);
    outFile.write((char*)(&image.header.idLength), 1);
    outFile.write((char*)(&image.header.colorMapType), 1);
    outFile.write((char*)(&image.header.dataTypeCode), 1);
    outFile.write((char*)(&image.header.colorMapOrigin), 2);
    outFile.write((char*)(&image.header.colorMapLength), 2);
    outFile.write((char*)(&image.header.colorMapDepth), 1);
    outFile.write((char*)(&image.header.xOrigin), 2);
    outFile.write((char*)(&image.header.yOrigin), 2);
    outFile.write((char*)(&image.header.width), 2);
    outFile.write((char*)(&image.header.height), 2);
    outFile.write((char*)(&image.header.bitsPerPixel), 1);
    outFile.write((char*)(&image.header.imageDescriptor), 1);
    for (auto ImagePixel : image.ImagePixels){
        outFile.write((char*)(&ImagePixel.Blue), 1);
        outFile.write((char*)(&ImagePixel.Green), 1);
        outFile.write((char*)(&ImagePixel.Red), 1);
    }
    outFile.close();
}
ImageData Multiply(const ImageData& top, const ImageData& bottom){
    ImageData OutputImage;
    OutputImage.header = top.header;
    int image_size = top.header.width * top.header.height;
    for (unsigned int i = 0; i < image_size; ++i){
        Pixel NewPixel;
        NewPixel.Blue = (top.ImagePixels[i].BlueNormal * bottom.ImagePixels[i].BlueNormal) * 255 +0.5f;
        NewPixel.Green = (top.ImagePixels[i].GreenNormal * bottom.ImagePixels[i].GreenNormal) * 255 +0.5f;
        NewPixel.Red = (top.ImagePixels[i].RedNormal * bottom.ImagePixels[i].RedNormal) * 255 +0.5f;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData Screen(const ImageData& top, const ImageData& bottom){
    ImageData OutputImage;
    OutputImage.header = top.header;
    int image_size = top.header.width * top.header.height;
    for (unsigned int i = 0; i < image_size; ++i){
        Pixel NewPixel;
        NewPixel.Blue = (1- (1-top.ImagePixels[i].BlueNormal) * (1-bottom.ImagePixels[i].BlueNormal)) * 255 +0.5f;
        NewPixel.Green = (1- (1-top.ImagePixels[i].GreenNormal) * (1-bottom.ImagePixels[i].GreenNormal)) * 255 +0.5f;
        NewPixel.Red = (1- (1-top.ImagePixels[i].RedNormal) * (1-bottom.ImagePixels[i].RedNormal)) * 255 +0.5f;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData Subtract(const ImageData& top, const ImageData& bottom){
    ImageData OutputImage;
    OutputImage.header = top.header;
    int image_size = top.header.width * top.header.height;
    for (unsigned int i = 0; i < image_size; ++i) {
        Pixel NewPixel;
        int BlueTemp = top.ImagePixels[i].Blue - bottom.ImagePixels[i].Blue;
        int GreenTemp = top.ImagePixels[i].Green - bottom.ImagePixels[i].Green;
        int RedTemp = top.ImagePixels[i].Red - bottom.ImagePixels[i].Red;
        if (BlueTemp > 255){
            BlueTemp = 255;
        }
        if (BlueTemp < 0){
            BlueTemp = 0;
        }
        if (GreenTemp > 255){
            GreenTemp = 255;
        }
        if (GreenTemp < 0){
            GreenTemp = 0;
        }
        if (RedTemp > 255){
            RedTemp = 255;
        }
        if (RedTemp < 0){
            RedTemp = 0;
        }
        NewPixel.Blue = BlueTemp;
        NewPixel.Green = GreenTemp;
        NewPixel.Red = RedTemp;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData Overlay(const ImageData& top, const ImageData& bottom){
    ImageData OutputImage;
    OutputImage.header = top.header;
    int image_size = top.header.width * top.header.height;
    for (unsigned int i = 0; i < image_size; ++i) {
        Pixel NewPixel;
        if (bottom.ImagePixels[i].BlueNormal <= 0.5){
            NewPixel.Blue = (2*top.ImagePixels[i].BlueNormal*bottom.ImagePixels[i].BlueNormal) * 255 + 0.5f;
        }
        if (bottom.ImagePixels[i].BlueNormal > 0.5){
            NewPixel.Blue = (1-(2*(1-top.ImagePixels[i].BlueNormal)*(1-bottom.ImagePixels[i].BlueNormal))) * 255 + 0.5f;
        }
        if (bottom.ImagePixels[i].GreenNormal <= 0.5){
            NewPixel.Green = (2*top.ImagePixels[i].GreenNormal*bottom.ImagePixels[i].GreenNormal) * 255 + 0.5f;
        }
        if (bottom.ImagePixels[i].GreenNormal > 0.5){
            NewPixel.Green = (1-(2*(1-top.ImagePixels[i].GreenNormal)*(1-bottom.ImagePixels[i].GreenNormal))) * 255 + 0.5f;
        }
        if (bottom.ImagePixels[i].RedNormal <= 0.5){
            NewPixel.Red = (2*top.ImagePixels[i].RedNormal*bottom.ImagePixels[i].RedNormal) * 255 + 0.5f;
        }
        if (bottom.ImagePixels[i].RedNormal > 0.5){
            NewPixel.Red = (1-(2*(1-top.ImagePixels[i].RedNormal)*(1-bottom.ImagePixels[i].RedNormal))) * 255 + 0.5f;
        }
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData AddBlue(const ImageData& image, const int& value){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        int TempBlue = (int)image.ImagePixels[i].Blue + value;
        if (TempBlue > 255){
            TempBlue = 255;
        }
        if (TempBlue < 0){
            TempBlue = 0;
        }
        NewPixel.Blue = TempBlue;
        NewPixel.Green = image.ImagePixels[i].Green;
        NewPixel.Red = image.ImagePixels[i].Red;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData AddGreen(const ImageData& image, const int& value){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Blue;
        int TempGreen = (int)image.ImagePixels[i].Green + value;
        if (TempGreen > 255){
            TempGreen = 255;
        }
        if (TempGreen < 0){
            TempGreen = 0;
        }
        NewPixel.Green = TempGreen;
        NewPixel.Red = image.ImagePixels[i].Red;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData AddRed(const ImageData& image, const int& value){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Blue;
        NewPixel.Green = image.ImagePixels[i].Green;
        int TempRed = (int)image.ImagePixels[i].Red + value;
        if (TempRed > 255){
            TempRed = 255;
        }
        if (TempRed < 0){
            TempRed = 0;
        }
        NewPixel.Red = TempRed;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData ScaleBlue(const ImageData& image, const int& scalar){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        int TempBlue = (image.ImagePixels[i].BlueNormal * scalar) * 255 + 0.5f;
        if (TempBlue > 255){
            TempBlue = 255;
        }
        if (TempBlue < 0){
            TempBlue = 0;
        }
        NewPixel.Blue = TempBlue;
        NewPixel.Green = image.ImagePixels[i].Green;
        NewPixel.Red = image.ImagePixels[i].Red;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData ScaleGreen(const ImageData& image, const int& scalar){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Blue;
        int TempGreen = (image.ImagePixels[i].GreenNormal * scalar) * 255 + 0.5f;
        if (TempGreen > 255){
            TempGreen = 255;
        }
        if (TempGreen < 0){
            TempGreen = 0;
        }
        NewPixel.Red = image.ImagePixels[i].Red;
        NewPixel.Green = TempGreen;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData ScaleRed(const ImageData& image, const int& scalar){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Blue;
        NewPixel.Green = image.ImagePixels[i].Green;
        int TempRed = (image.ImagePixels[i].RedNormal * scalar) * 255 + 0.5f;
        if (TempRed > 255){
            TempRed = 255;
        }
        if (TempRed < 0){
            TempRed = 0;
        }
        NewPixel.Red = TempRed;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData BlueChannel(const ImageData& image){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Blue;
        NewPixel.Green = image.ImagePixels[i].Blue;
        NewPixel.Red = image.ImagePixels[i].Blue;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData GreenChannel(const ImageData& image){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Green;
        NewPixel.Green = image.ImagePixels[i].Green;
        NewPixel.Red = image.ImagePixels[i].Green;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData RedChannel(const ImageData& image){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = 0; i < image.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = image.ImagePixels[i].Red;
        NewPixel.Green = image.ImagePixels[i].Red;
        NewPixel.Red = image.ImagePixels[i].Red;
        NewPixel.BlueNormal = (float)NewPixel.Blue/255;
        NewPixel.GreenNormal = (float)NewPixel.Green/255;
        NewPixel.RedNormal = (float)NewPixel.Red/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData CombineChannels(const ImageData& blue_layer, const ImageData& green_layer, const ImageData& red_layer){
    ImageData OutputImage;
    OutputImage.header = blue_layer.header;
    for (int i = 0; i < blue_layer.ImagePixels.size(); ++i){
        Pixel NewPixel;
        NewPixel.Blue = blue_layer.ImagePixels[i].Blue;
        NewPixel.Green = green_layer.ImagePixels[i].Green;
        NewPixel.Red = red_layer.ImagePixels[i].Red;
        NewPixel.BlueNormal = float(NewPixel.Blue)/255;
        NewPixel.GreenNormal = float(NewPixel.Green)/255;
        NewPixel.RedNormal = float(NewPixel.Red)/255;
        OutputImage.ImagePixels.push_back(NewPixel);
    }
    return OutputImage;
}
ImageData Rotate180(const ImageData& image){
    ImageData OutputImage;
    OutputImage.header = image.header;
    for (int i = (int)image.ImagePixels.size() - 1; i >=0; --i){
        OutputImage.ImagePixels.push_back(image.ImagePixels[i]);
    }
    return OutputImage;
}
int main(int argc, char* argv[]){
    ImageData TrackingImage;
    int ArgCounter = 0;
    vector<string> Method2Images{"multiply", "subtract", "overlay", "screen", "combine"};
    vector<string> Method1Image{"flip", "onlyred", "onlygreen", "onlyblue"};
    vector<string> MethodParameter{"addred", "addgreen", "addblue","scalered", "scalegreen", "scaleblue"};
        if (argc == 1){
            cout << "Image Modifier: follow the format below" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "\t" << "./project2.out [output] [firstImage] [method] [...]" << endl;
            return 0;
        }
        if (argc == 2 && strcmp(argv[1], "--help") == 0){
            cout << "Image Modifier: follow the format below" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "\t" << "./project2.out [output] [firstImage] [method] [...]" << endl;
            return 0;
        }
        else{
            if (argc == 2){
                cout << "Invalid file name." << endl;
                return 0;
            }
            if (argc == 3 && strstr(argv[2], ".tga") == nullptr){
                cout << "Invalid file name." << endl;
                return 0;
            }
            else{
                if (strstr(argv[2], ".tga") == nullptr){
                    cout << "Invalid file name." << endl;
                    return 0;
                }
                string firstImage = argv[2];
                ifstream inFile(firstImage, ios_base::binary);
                if (inFile.is_open()){
                    inFile.close();
                    TrackingImage = OpenAndStore(firstImage);
                }
                else{
                    cout << "File does not exist." << endl;
                    return 0;
                }
            }
            if (argc >=4 && strstr(argv[1], ".tga") == nullptr){
                cout << "Invalid file name." << endl;
                return 0;
            }
            if (argc >=4 && strstr(argv[2], ".tga") == nullptr){
                cout << "Invalid file name." << endl;
                return 0;
            }
            else if (argc >= 4){
                string firstImage = argv[2];
                ifstream inFile(firstImage, ios_base::binary);
                if (inFile.is_open()){
                    inFile.close();
                    TrackingImage = OpenAndStore(firstImage);
                }
                else{
                    cout << "File does not exist." << endl;
                    return 0;
                }
            }
            if (argc >= 4){
                string method = argv[3];
                vector<string>::iterator i;
                i = find(Method2Images.begin(), Method2Images.end(), method);
                vector<string>::iterator j;
                j = find(Method1Image.begin(), Method1Image.end(), method);
                vector<string>::iterator k;
                k = find(MethodParameter.begin(), MethodParameter.end(), method);
                if (i != Method2Images.end()){
                    try{
                        string secondImage = argv[4];
                    }
                    catch(...){
                        cout << "Missing argument." << endl;
                        return 0;
                    }
                    if (strstr(argv[4], ".tga") == nullptr){
                        cout << "Invalid argument, invalid file name." << endl;
                        return 0;
                    }
                    string secondImage = argv[4];
                    ifstream inFile(secondImage, ios_base::binary);
                    if (inFile.is_open()){
                        inFile.close();
                        ImageData SecondImage = OpenAndStore(secondImage);
                        if (method == "multiply"){
                            TrackingImage = Multiply(TrackingImage, SecondImage);
                            cout << "...multiplying two images..." << endl;
                            ArgCounter = 5;
                        }
                        else if (method == "subtract"){
                            TrackingImage = Subtract(TrackingImage, SecondImage);
                            cout << "...subtracting two images..." << endl;
                            ArgCounter = 5;
                        }
                        else if (method == "overlay"){
                            TrackingImage = Overlay(TrackingImage, SecondImage);
                            cout << "...overlaying two images..." << endl;
                            ArgCounter = 5;
                        }
                        else if (method == "screen"){
                            TrackingImage = Screen(TrackingImage, SecondImage);
                            cout << "...screening two images..." << endl;
                            ArgCounter = 5;
                        }
                        else if (method == "combine"){
                            if (strstr(argv[4], ".tga") == nullptr){
                                cout << "Invalid argument, invalid file name." << endl;
                                return 0;
                            }
                            if (strstr(argv[5], ".tga") == nullptr){
                                cout << "Invalid argument, invalid file name." << endl;
                                return 0;
                            }
                            ifstream inFile2(argv[4], ios_base::binary);
                            if (inFile2.is_open()){
                                inFile2.close();
                                ImageData secondlayer = OpenAndStore(argv[4]);
                                ifstream inFile3(argv[5], ios_base::binary);
                                if (inFile3.is_open()){
                                    inFile3.close();
                                    ImageData thirdlayer = OpenAndStore(argv[5]);
                                    TrackingImage = CombineChannels(thirdlayer, secondlayer, TrackingImage);
                                    cout << "...combining three images..." << endl;
                                    ArgCounter = 6;
                                }
                                else{
                                    cout << "Invalid argument, file does not exist." << endl;
                                    return 0;
                                }
                            }
                            else{
                                cout << "Invalid argument, file does not exist." << endl;
                                return 0;
                            }
                        }
                    }
                    else{
                        cout << "Invalid argument, file does not exist." << endl;
                        return 0;
                    }
                }
                else if (j != Method1Image.end()){
                    if (method == "flip"){
                        TrackingImage = Rotate180(TrackingImage);
                        cout << "...flipping..." << endl;
                        ArgCounter = 4;
                    }
                    else if (method == "onlyred"){
                        TrackingImage = RedChannel(TrackingImage);
                        cout << "...committing red channel..." << endl;
                        ArgCounter = 4;
                    }
                    else if (method == "onlygreen"){
                        TrackingImage = GreenChannel(TrackingImage);
                        cout << "...committing green channel..." << endl;
                        ArgCounter = 4;
                    }
                    else if (method == "onlyblue"){
                        TrackingImage = BlueChannel(TrackingImage);
                        cout << "...committing blue channel..." << endl;
                        ArgCounter = 4;
                    }
                }
                else if (k != MethodParameter.end()){
                    int parameter;
                    try{
                        parameter = stoi(argv[4]);
                    }
                    catch (invalid_argument){
                        cout << "Invalid argument, expected number." << endl;
                        return 0;
                    }
                    catch (...){
                        cout << "Missing argument." << endl;
                        return 0;
                    }
                    if (method == "addred"){
                        TrackingImage = AddRed(TrackingImage, parameter);
                        cout << "...adding red..." << endl;
                        ArgCounter = 5;
                    }
                    else if (method == "addgreen"){
                        TrackingImage = AddGreen(TrackingImage, parameter);
                        cout << "...adding green..." << endl;
                        ArgCounter = 5;
                    }
                    else if (method == "addblue"){
                        TrackingImage = AddBlue(TrackingImage, parameter);
                        cout << "...adding blue..." << endl;
                        ArgCounter = 5;
                    }
                    else if (method == "scalered"){
                        TrackingImage = ScaleRed(TrackingImage, parameter);
                        cout << "...scaling red..." << endl;
                        ArgCounter = 5;
                    }
                    else if (method == "scalegreen"){
                        TrackingImage = ScaleGreen(TrackingImage, parameter);
                        cout << "...scaling green..." << endl;
                        ArgCounter = 5;
                    }
                    else if (method == "scaleblue"){
                        TrackingImage = ScaleBlue(TrackingImage, parameter);
                        cout << "...scaling blue..." << endl;
                        ArgCounter = 5;
                    }
                }
                else{
                    cout << "Invalid method name." << endl;
                    return 0;
                }
            }
        }
    while (ArgCounter < argc){
        string method = argv[ArgCounter];
        vector<string>::iterator i;
        i = find(Method2Images.begin(), Method2Images.end(), method);
        vector<string>::iterator j;
        j = find(Method1Image.begin(), Method1Image.end(), method);
        vector<string>::iterator k;
        k = find(MethodParameter.begin(), MethodParameter.end(), method);
        if (i != Method2Images.end()){
            string secondImage = argv[ArgCounter + 1];
            if (strstr(argv[ArgCounter + 1], ".tga") == nullptr){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            ifstream inFile(secondImage, ios_base::binary);
            if (inFile.is_open()){
                inFile.close();
                ImageData SecondImage = OpenAndStore(secondImage);
                if (method == "multiply"){
                    TrackingImage = Multiply(TrackingImage, SecondImage);
                    cout << "...multiplying two images..." << endl;
                    ArgCounter += 2;
                }
                else if (method == "subtract"){
                    TrackingImage = Subtract(TrackingImage, SecondImage);
                    cout << "...subtracting two images..." << endl;
                    ArgCounter += 2;
                }
                else if (method == "overlay"){
                    TrackingImage = Overlay(TrackingImage, SecondImage);
                    cout << "...overlaying two images..." << endl;
                    ArgCounter += 2;
                }
                else if (method == "screen"){
                    TrackingImage = Screen(TrackingImage, SecondImage);
                    cout << "...screening two images..." << endl;
                    ArgCounter += 2;
                }
                else if (method == "combine"){
                    if (strstr(argv[ArgCounter + 1], ".tga") == nullptr){
                        cout << "Invalid argument, invalid file name." << endl;
                        return 0;
                    }
                    if (strstr(argv[ArgCounter + 2], ".tga") == nullptr){
                        cout << "Invalid argument, invalid file name." << endl;
                        return 0;
                    }
                    ifstream inFile2(argv[ArgCounter + 1], ios_base::binary);
                    if (inFile2.is_open()){
                        inFile2.close();
                        ImageData secondlayer = OpenAndStore(argv[ArgCounter + 1]);
                        ifstream inFile3(argv[ArgCounter + 2], ios_base::binary);
                        if (inFile3.is_open()){
                            inFile3.close();
                            ImageData thirdlayer = OpenAndStore(argv[ArgCounter + 2]);
                            TrackingImage = CombineChannels(thirdlayer, secondlayer, TrackingImage);
                            cout << "...combining three images..." << endl;
                            ArgCounter += 3;
                        }
                        else{
                            cout << "Invalid argument, file does not exist." << endl;
                            return 0;
                        }
                    }
                    else{
                        cout << "Invalid argument, file does not exist." << endl;
                        return 0;
                    }
                }
            }
            else{
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
        }
        else if (j != Method1Image.end()){
            if (method == "flip"){
                TrackingImage = Rotate180(TrackingImage);
                cout << "...flipping..." << endl;
                ArgCounter += 1;
            }
            else if (method == "onlyred"){
                TrackingImage = RedChannel(TrackingImage);
                cout << "...committing red channel..." << endl;
                ArgCounter += 1;
            }
            else if (method == "onlygreen"){
                TrackingImage = GreenChannel(TrackingImage);
                cout << "...committing green channel..." << endl;
                ArgCounter += 1;
            }
            else if (method == "onlyblue"){
                TrackingImage = BlueChannel(TrackingImage);
                cout << "...committing blue channel..." << endl;
                ArgCounter += 1;
            }
        }
        else if (k != MethodParameter.end()){
            int parameter;
            try{
                parameter = stoi(argv[ArgCounter + 1]);
            }
            catch (invalid_argument){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            catch (...){
                cout << "Missing argument." << endl;
                return 0;
            }
            if (method == "addred"){
                TrackingImage = AddRed(TrackingImage, parameter);
                cout << "...adding red..." << endl;
                ArgCounter += 2;
            }
            else if (method == "addgreen"){
                TrackingImage = AddGreen(TrackingImage, parameter);
                cout << "...adding green..." << endl;
                ArgCounter += 2;
            }
            else if (method == "addblue"){
                TrackingImage = AddBlue(TrackingImage, parameter);
                cout << "...adding blue..." << endl;
                ArgCounter += 2;
            }
            else if (method == "scalered"){
                TrackingImage = ScaleRed(TrackingImage, parameter);
                cout << "...scaling red..." << endl;
                ArgCounter += 2;
            }
            else if (method == "scalegreen"){
                TrackingImage = ScaleGreen(TrackingImage, parameter);
                cout << "...scaling green..." << endl;
                ArgCounter += 2;
            }
            else if (method == "scaleblue"){
                TrackingImage = ScaleBlue(TrackingImage, parameter);
                cout << "...scaling blue..." << endl;
                ArgCounter += 2;
            }
        }
        else{
            cout << "Invalid method name." << endl;
            return 0;
        }
    }
    string OutputFile = argv[1];
    WriteFile(TrackingImage, OutputFile);
}
