#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace tr {

struct Pixel { std::uint8_t b, g, r; };

class Image {
public:
    int width, height;
    std::vector<Pixel> pixels;
    Image(int width, int height);
    void set(int x, int y, Pixel p);
    void write(const std::string &filePath);
    bool read(const std::string &filePath);
    Pixel get(int x, int y)const;
};

}