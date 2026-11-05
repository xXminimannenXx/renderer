#pragma once
#include <vector>
#include "raylib.h"
#include "Image.h"
namespace tr{
void toRaylibPixels(const tr::Image &source, std::vector<Color> &out);
}