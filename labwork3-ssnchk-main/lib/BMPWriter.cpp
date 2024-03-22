#include <iostream>
#include <string>
#include <fstream>
#include "SandPile.h"

#pragma pack(push, 1)

struct BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};


#pragma pack(pop)

void Image::Export(const char* name, Sandpile sandpile) const {

    Palette palete;

    std::ofstream file(name, std::ios::binary);

    uint32_t rowSize = m_width / 2 + m_width % 2;
    uint32_t paddingPerRow = (4 - rowSize % 4) % 4;
    uint32_t totalDataSize = (rowSize + paddingPerRow) * m_height;

    BITMAPFILEHEADER fileHeader = {0};
    BITMAPINFOHEADER infoHeader = {0};

    fileHeader.bfType = 0x4D42;
    fileHeader.bfSize = totalDataSize + sizeof(palete) + sizeof(infoHeader) + sizeof(fileHeader);
    fileHeader.bfOffBits = sizeof(palete) + sizeof(infoHeader) + sizeof(fileHeader);

    infoHeader.biSize = sizeof(infoHeader);
    infoHeader.biWidth = static_cast<uint8_t>(m_width);
    infoHeader.biHeight = static_cast<uint8_t>(m_height);
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 4;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = totalDataSize;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 5;
    infoHeader.biClrImportant = 5;

    file.write((char *) &fileHeader, sizeof(fileHeader));
    file.write((char *) &infoHeader, sizeof(infoHeader));
    file.write((char *) palete.all_colors, sizeof(palete));
    auto* pixelData = new uint8_t [totalDataSize];
    uint8_t * currentPixel = pixelData;
    for (int y = m_height - 1; y >= 0; --y) {
        for (int x = 0; x < m_width; x += 2) {
            uint64_t first_pixel = sandpile.get_value(y, x);
            uint64_t second_pixel = 4;
            if (x + 1 < m_width) {
                second_pixel = std::min((long long) sandpile.get_value(y, x + 1), 4ll);
            }
            *currentPixel = static_cast<uint8_t>(first_pixel << 4 | second_pixel);
            ++currentPixel;
        }
        currentPixel += paddingPerRow;
    }
    file.write((char*) (pixelData), totalDataSize);
    delete[] pixelData;
}
