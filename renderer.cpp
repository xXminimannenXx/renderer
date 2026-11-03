#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <limits>

struct Pixel
{

    std::uint8_t b;
    std::uint8_t g;
    std::uint8_t r;
};
struct Vector2
{

    int x;
    int y;
};
struct Vector3f
{
    float x;
    float y;
    float z;
};
struct ScreenPoint
{

    Vector2 pos;
    float z;
};
class Image
{

public:
    int width;
    int height;

    std::vector<Pixel> pixels;
    Image(int width, int height)
    {
        this->width = width;
        this->height = height;
        pixels.resize(width * height);
    }
    void set(int x, int y, Pixel p)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return;

        pixels[y * width + x] = p;
    }
    void write(const std::string &filePath)
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
};
void line(Vector2 a, Vector2 b, Image &image, Pixel p)
{

    bool steep = false;

    if (std::abs(b.y - a.y) > std::abs(b.x - a.x))
        steep = true;
    if (steep)
    {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }
    if (a.x > b.x)
    {
        std::swap(a, b);
    }

    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int errorStep = 2 * std::abs(dy);
    int errorReset = 2 * dx;
    int y = a.y;
    int error = 0;
    int dir = b.y > a.y ? 1 : -1;
    for (int x = a.x; x <= b.x; x++)
    {

        if (!steep)
        {
            image.set(x, y, p);
        }
        else
        {
            image.set(y, x, p);
        }
        error += errorStep;
        if (error > dx)
        {
            y += dir;
            error -= errorReset;
        }
    }
}
class Model
{

public:
    std::vector<Vector3f> verts;
    std::vector<int> trig;
    Model(const std::string &filePath)
    {
        read(filePath);
    }

private:
    void read(const std::string &filePath)
    {

        std::ifstream fi(filePath);
        if (!fi.is_open())
        {
            std::cerr << "file cant open\n";
            return;
        }
        std::string line;
        while (std::getline(fi, line))
        {

            std::istringstream si(line);
            std::string word;
            si >> word;
            if (word == "v")
            {
                Vector3f v;
                si >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
            if (word == "f")
            {
                for (int i = 0; i < 3; i++)
                {
                    std::string group;
                    si >> group;
                    int index = std::stoi(group) - 1;
                    trig.push_back(index);
                }
            }
        }
    }
};
ScreenPoint toScreen(Vector3f v, int width, int height)
{

    return {static_cast<int>((v.x + 1) * width) / 2, static_cast<int>((v.y + 1) * height / 2), v.z};
}
void drawWireFrame(const Model &model, Image &image, Pixel color)
{
    for (int i = 0; i < model.trig.size() / 3; i++)
    {

        int i0 = model.trig.at(3 * i);
        int i1 = model.trig.at(3 * i + 1);
        int i2 = model.trig.at(3 * i + 2);
        Vector3f a = {model.verts.at(i0)};
        Vector3f b = {model.verts.at(i1)};
        Vector3f c = {model.verts.at(i2)};
        line(toScreen(a, image.width, image.height).pos, toScreen(b, image.width, image.height).pos, image, color);
        line(toScreen(b, image.width, image.height).pos, toScreen(c, image.width, image.height).pos, image, color);
        line(toScreen(c, image.width, image.height).pos, toScreen(a, image.width, image.height).pos, image, color);
    }
}
float areaTriangle(Vector2 a, Vector2 b, Vector2 c)
{

    return 0.5 * ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
}
void drawTriangle(Vector2 a, Vector2 b, Vector2 c, Image &image, Pixel color)
{

    float area = areaTriangle(a, b, c);
    if (area == 0)
        return;

    int minX = std::min({a.x, b.x, c.x});
    int maxX = std::max({a.x, b.x, c.x});
    int minY = std::min({a.y, b.y, c.y});
    int maxY = std::max({a.y, b.y, c.y});
    minX = minX < 0 ? 0 : minX;
    maxX = maxX > image.width - 1 ? image.width - 1 : maxX;
    minY = minY < 0 ? 0 : minY;
    maxY = maxY > image.height - 1 ? image.height - 1 : maxY;

    for (int y = minY; y <= maxY; y++)
    {

        for (int x = minX; x <= maxX; x++)
        {
            Vector2 p = {x, y};
            float alfa = areaTriangle(p, b, c) / area;
            float beta = areaTriangle(a, p, c) / area;
            float gamma = areaTriangle(a, b, p) / area;

            if (alfa >= 0 && beta >= 0 && gamma >= 0)
            {

                image.set(x, y, color);
            }
        }
    }
}
void drawModel(const Model &model, Image &image, Pixel color)
{
    // color används inte rn
    for (int i = 0; i < model.trig.size() / 3; i++)
    {

        int i0 = model.trig.at(3 * i);
        int i1 = model.trig.at(3 * i + 1);
        int i2 = model.trig.at(3 * i + 2);
        Vector3f a = {model.verts.at(i0)};
        Vector3f b = {model.verts.at(i1)};
        Vector3f c = {model.verts.at(i2)};
        drawTriangle(toScreen(a, image.width, image.height).pos, toScreen(b, image.width, image.height).pos, toScreen(c, image.width, image.height).pos, image, {static_cast<uint8_t>(std::rand() % 256), static_cast<uint8_t>(std::rand() % 256), static_cast<uint8_t>(std::rand() % 256)});
    }
}
int main()
{

    Image image(1600, 1600);
    Model model("african_head/african_head.obj");
    std::vector<float> zBuf(image.height * image.width, -std::numeric_limits<float>::max());
    // drawWireFrame(model, image,{255,0,0});
    drawModel(model, image, {255, 0, 0});
    image.write("output.tga");
}