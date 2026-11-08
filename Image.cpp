#include "Image.h"
#include <fstream>
#include <iostream>
#include <utility>

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

    bool Image::read(const std::string &filePath)
    {

        std::ifstream fi(filePath, std::ios::binary);
        std::uint8_t header[18]{};
        if (!fi.is_open())
        {
            std::cerr << "cant open file\n";
            return false;
        }
        fi.read(reinterpret_cast<char *>(header), 18);
        if (!fi)
            return false;
        if (header[2] != 2 && header[2] != 10)
        {
            return false;
        }
        uint8_t imageType = header[2];
        this->width = header[12] + header[13] * 256;
        this->height = header[14] + header[15] * 256;
        int bytePerPixels{};
        if (header[16] / 8 != 3 && header[16] / 8 != 4)
        {
            return false;
        }
        else
        {
            bytePerPixels = header[16] / 8;
        }
        bool topToBottom = (header[17] & 32) != 0;

        pixels.resize(width * height);
        int bytesToJump = header[0];
        fi.ignore(bytesToJump);
        if (imageType == 2 && bytePerPixels == 3)
        {
            fi.read(reinterpret_cast<char *>(pixels.data()), pixels.size() * sizeof(Pixel));
        }
        else if (imageType == 2 && bytePerPixels == 4)
        {
            for (int i = 0; i < pixels.size(); i++)
            {
                uint8_t cur[4]{};
                fi.read(reinterpret_cast<char *>(cur), 4);
                pixels[i] = {cur[0], cur[1], cur[2]};
            }
        }
        else if (imageType == 10)
        {
            int count = 0;

            while (count < width * height)
            {
                uint8_t tempByte;
                int n = 0;

                fi.read(reinterpret_cast<char *>(&tempByte), 1);
                if (!fi)
                    return false;
                if (tempByte >= 128)
                    n = tempByte - 128 + 1;
                else
                    n = tempByte + 1;
                if (count + n > width * height)
                    return false;
                if (tempByte >= 128)
                {
                    uint8_t cur[4]{};
                    fi.read(reinterpret_cast<char *>(cur), bytePerPixels);
                    for (int i = 0; i < n; i++)
                    {
                        pixels[count + i] = {
                            cur[0], cur[1], cur[2]};
                    }
                }
                else{
                    for(int i = 0; i < n; i++){
                    uint8_t cur[4]{};
                    fi.read(reinterpret_cast<char *>(cur), bytePerPixels);
                    pixels[count + i] = {cur[0],cur[1],cur[2]};
                    }
                }
                count += n;
            }
        }

        if (!fi)
            return false;
        if(topToBottom){
            for(int y = 0; y < height/2; y++){
                for(int x = 0; x < width; x++){
                    std::swap(pixels[y * width + x], pixels[(height -1 -y) * width + x]);
                }
            }
        }
        return true;
    }
    Pixel Image::get(int x,int y) const {

        if (x < 0 || y < 0 || x >= width || y >= height)
            return {0,0,0};
        return pixels[y * width + x];
    }
}