#pragma once
#include <vector>
#include "Geometry.h"
#include "Image.h"
#include "Model.h"

namespace tr
{

    ScreenPoint toScreen(Vector3f v, int width, int height, Vector3f normal = {0, 0, 0});

    void line(Vector2 a, Vector2 b, Image &image, Pixel p);

    float areaTriangle(Vector2 a, Vector2 b, Vector2 c);

    void drawTriangle(ScreenPoint a, ScreenPoint b, ScreenPoint c, Image &image, Pixel color,
                      std::vector<float> &zBuf, Vector3f lightDir);

    void drawWireFrame(const Model &model, Image &image, Pixel color);

    void drawModel(const Model &model, Image &image, Pixel color, std::vector<float> &zBuf,
                   Vector3f lightDir, Vector3f viewDir = {0, 0, 1});

}