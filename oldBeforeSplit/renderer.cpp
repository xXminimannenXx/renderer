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
#include <cmath>


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
    Vector3f normal;
};
Vector3f v3f_Sub(Vector3f a, Vector3f b)
{

    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
Vector3f v3f_cross(Vector3f a, Vector3f b)
{

    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
float v3f_dot(Vector3f a, Vector3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vector3f v3f_norm(Vector3f a)
{
    float leng = std::sqrt(v3f_dot(a, a));
    if (leng == 0)
        return a;
    return {a.x / leng, a.y / leng, a.z / leng};
}
Vector3f v3f_add(Vector3f a, Vector3f b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Vector3f v3f_scale(Vector3f a, float s)
{
    return {a.x * s, a.y * s, a.z * s};
}

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
    std::vector<Vector3f> norms;
    std::vector<int> normIdx;
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
            if (word == "vn")
            {
                Vector3f v;
                si >> v.x >> v.y >> v.z;
                norms.push_back(v);
            }
            if (word == "f")
            {

                for (int i = 0; i < 3; i++)
                {
                    std::string group;
                    si >> group;
                    std::replace(group.begin(), group.end(), '/', ' ');
                    int pos;
                    int texCord;
                    int norm;
                    std::istringstream cS(group);
                    cS >> pos >> texCord >> norm;
                    trig.push_back(pos - 1);
                    normIdx.push_back(norm - 1);
                }
            }
        }
    }
};
ScreenPoint toScreen(Vector3f v, int width, int height, Vector3f normal = {0, 0, 0})
{

    return {static_cast<int>((v.x + 1) * width) / 2, static_cast<int>((v.y + 1) * height / 2), v.z, normal};
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
void drawTriangle(ScreenPoint a, ScreenPoint b, ScreenPoint c, Image &image, Pixel color, std::vector<float> &zBuf, Vector3f lightDir)
{

    float area = areaTriangle(a.pos, b.pos, c.pos);
    if (area == 0)
        return;

    int minX = std::min({a.pos.x, b.pos.x, c.pos.x});
    int maxX = std::max({a.pos.x, b.pos.x, c.pos.x});
    int minY = std::min({a.pos.y, b.pos.y, c.pos.y});
    int maxY = std::max({a.pos.y, b.pos.y, c.pos.y});
    minX = minX < 0 ? 0 : minX;
    maxX = maxX > image.width - 1 ? image.width - 1 : maxX;
    minY = minY < 0 ? 0 : minY;
    maxY = maxY > image.height - 1 ? image.height - 1 : maxY;

    for (int y = minY; y <= maxY; y++)
    {

        for (int x = minX; x <= maxX; x++)
        {
            Vector2 p = {x, y};
            float alfa = areaTriangle(p, b.pos, c.pos) / area;
            float beta = areaTriangle(a.pos, p, c.pos) / area;
            float gamma = areaTriangle(a.pos, b.pos, p) / area;

            if (alfa >= 0 && beta >= 0 && gamma >= 0)
            {
                float pD = alfa * a.z + beta * b.z + gamma * c.z;
                int indx = y * image.width + x;
                if (pD > zBuf[indx])
                {

                    Vector3f normal = v3f_norm(v3f_add(v3f_scale(a.normal, alfa), v3f_add(v3f_scale(b.normal, beta), v3f_scale(c.normal, gamma))));
                    float intensity = std::max(0.0f, v3f_dot(normal, lightDir));
                    Pixel drawColor = {static_cast<uint8_t>(color.b * intensity), static_cast<uint8_t>(color.g * intensity), static_cast<uint8_t>(color.r * intensity)};
                    zBuf[indx] = pD;
                    image.set(x, y, drawColor);
                }
            }
        }
    }
}
void drawModel(const Model &model, Image &image, Pixel color, std::vector<float> &zBuf, Vector3f lightDir, Vector3f viewDir = {0, 0, 1})
{

    for (int i = 0; i < model.trig.size() / 3; i++)
    {
        int n0 = model.normIdx.at(3 * i);
        int n1 = model.normIdx.at(3 * i + 1);
        int n2 = model.normIdx.at(3 * i + 2);
        Vector3f aNormal = model.norms.at(n0);
        Vector3f bNormal = model.norms.at(n1);
        Vector3f cNormal = model.norms.at(n2);

        int i0 = model.trig.at(3 * i);
        int i1 = model.trig.at(3 * i + 1);
        int i2 = model.trig.at(3 * i + 2);
        Vector3f a = model.verts.at(i0);
        Vector3f b = model.verts.at(i1);
        Vector3f c = model.verts.at(i2);
        Vector3f norm = v3f_norm(v3f_cross(v3f_Sub(b, a), v3f_Sub(c, a)));

        float facing = v3f_dot(norm, viewDir);
        if (facing <= 0)
            continue;

        drawTriangle(toScreen(a, image.width, image.height, aNormal), toScreen(b, image.width, image.height, bNormal), toScreen(c, image.width, image.height, cNormal), image, color, zBuf, lightDir);
    }
}

int main()
{
    Vector3f lightDir = {0, 0, 1};
    Image image(1600, 1600);
    Model model("african_head/african_head.obj");
    Model model1("african_head/african_head_eye_outer.obj");
    Model model2("african_head/african_head_eye_inner.obj");
    std::vector<float> zBuf(image.height * image.width, -std::numeric_limits<float>::max());

    drawModel(model, image, {255, 255, 255}, zBuf, lightDir);
    drawModel(model1, image, {255, 255, 255}, zBuf, lightDir);
    drawModel(model2, image, {255, 255, 255}, zBuf, lightDir);

    image.write("output.tga");
}