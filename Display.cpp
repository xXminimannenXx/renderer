#include "Display.h"

void toRaylibPixels(const tr::Image &source, std::vector<Color> &out)
{

    out.resize(source.width * source.height);
    for (int y = 0; y < source.height; y++)
    {
        for (int x = 0; x < source.width; x++)
        {

            const tr::Pixel &p = source.pixels[y * source.width + x];
            out[(source.height - 1 - y) * source.width + x] = {p.r, p.g, p.b, 255};
        }
    }
}