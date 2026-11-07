#include "Draw.h"
#include <algorithm>
#include <cstdint>
#include <utility>

namespace tr
{

    ScreenPoint toScreen(Vector3f v, int width, int height, Vector3f normal)
    {
        return {static_cast<int>((v.x + 1) * width) / 2, static_cast<int>((v.y + 1) * height / 2), v.z, normal};
    }

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

    float areaTriangle(Vector2 a, Vector2 b, Vector2 c)
    {
        return 0.5 * ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
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

    void drawTriangle(ScreenPoint a, ScreenPoint b, ScreenPoint c, Image &image, Pixel color,
                      std::vector<float> &zBuf, Vector3f lightDir)
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
                        Pixel drawColor = {static_cast<std::uint8_t>(color.b * intensity), static_cast<std::uint8_t>(color.g * intensity), static_cast<std::uint8_t>(color.r * intensity)};
                        zBuf[indx] = pD;
                        image.set(x, y, drawColor);
                    }
                }
            }
        }
    }

    void drawModel(const Model &model, Image &image, Pixel color, std::vector<float> &zBuf,
                   Vector3f lightDir, float yaw, float pitch, const tr::Camera &cam)
    {
        for (int i = 0; i < model.trig.size() / 3; i++)
        {
            int n0 = model.normIdx.at(3 * i);
            int n1 = model.normIdx.at(3 * i + 1);
            int n2 = model.normIdx.at(3 * i + 2);
            Vector3f aNormal = model.norms.at(n0);
            Vector3f bNormal = model.norms.at(n1);
            Vector3f cNormal = model.norms.at(n2);
            aNormal = tr::v3f_rotateY(aNormal, yaw);
            bNormal = tr::v3f_rotateY(bNormal, yaw);
            cNormal = tr::v3f_rotateY(cNormal, yaw);

            aNormal = tr::v3f_rotateX(aNormal, pitch);
            bNormal = tr::v3f_rotateX(bNormal, pitch);
            cNormal = tr::v3f_rotateX(cNormal, pitch);

            int i0 = model.trig.at(3 * i);
            int i1 = model.trig.at(3 * i + 1);
            int i2 = model.trig.at(3 * i + 2);
            Vector3f a = model.verts.at(i0);
            Vector3f b = model.verts.at(i1);
            Vector3f c = model.verts.at(i2);
            a = tr::v3f_rotateY(a, yaw);
            b = tr::v3f_rotateY(b, yaw);
            c = tr::v3f_rotateY(c, yaw);

            a = tr::v3f_rotateX(a, pitch);
            b = tr::v3f_rotateX(b, pitch);
            c = tr::v3f_rotateX(c, pitch);
            Vector3f norm = v3f_norm(v3f_cross(v3f_Sub(b, a), v3f_Sub(c, a)));

            float facing = v3f_dot(norm, cam.directionTo(a));
            if (facing <= 0)
                continue;
            a = cam.toView(a);
            b = cam.toView(b);
            c = cam.toView(c);

            if (a.z > -0.1 || b.z > -0.1 || c.z > -0.1)
                continue;
            
            a = cam.project(a);
            b = cam.project(b);
            c = cam.project(c);
            drawTriangle(toScreen(a, image.width, image.height, aNormal),
                         toScreen(b, image.width, image.height, bNormal),
                         toScreen(c, image.width, image.height, cNormal),
                         image, color, zBuf, lightDir);
        }
    }

}