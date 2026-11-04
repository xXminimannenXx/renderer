#include <raylib.h>
#include <limits>
#include <vector>
#include "Draw.h"
#include "Image.h"
#include "Model.h"

int main()
{
    tr::Vector3f lightDir = {0, 0, 1};
    tr::Image image(1600, 1600);
    tr::Model model("african_head/african_head.obj");
    tr::Model model1("african_head/african_head_eye_outer.obj");
    tr::Model model2("african_head/african_head_eye_inner.obj");
    std::vector<float> zBuf(image.height * image.width, -std::numeric_limits<float>::max());

    tr::drawModel(model, image, {255, 255, 255}, zBuf, lightDir);
    tr::drawModel(model1, image, {255, 255, 255}, zBuf, lightDir);
    tr::drawModel(model2, image, {255, 255, 255}, zBuf, lightDir);

    image.write("output.tga");
}