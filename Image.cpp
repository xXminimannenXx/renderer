#include "image.h"
#include <fstream>
#include <iostream>

namespace tr
{

    Image::Image(int width, int height)
    {
        this->width = width;
        this->height = height;
        pixels.resize(width * height);
    }

    void Image::set(int x, int y, Pixel p)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return;

        pixels[y * width + x] = p;
    }

    void Image::write(const std::string &filePath)
    {
        std::ofstream fo(filePath, std::ios::binary);
        if (!fo.is_open())
        {
            std::cerr << "cant open file\n";
            return;
        }
        std::uint8_t header[18]{};
        header[2] = 2;
        header[12] = width % 256;
        header[13] = width / 256;
        header[14] = height % 256;
        header[15] = height / 256;
        header[16] = 24;

        fo.write(reinterpret_cast<const char *>(header), 18);
        fo.write(reinterpret_cast<const char *>(pixels.data()), pixels.size() * sizeof(Pixel));
    }

}