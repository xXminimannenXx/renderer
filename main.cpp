#include <raylib.h>
#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Draw.h"
#include "Image.h"
#include "Model.h"
#include "Display.h"

int main()
{
    tr::Vector3f lightDir = {0, 0, 1};

    tr::Image image(1200, 1200);

    tr::Model model("african_head/african_head.obj");
    tr::Model model1("african_head/african_head_eye_outer.obj");
    tr::Model model2("african_head/african_head_eye_inner.obj");

    std::vector<float> zBuf(image.height * image.width, -std::numeric_limits<float>::max());
    std::vector<Color> screenPixels;


    InitWindow(image.width, image.height, "Renderer");
    SetTargetFPS(60);

    Image image2 = GenImageColor(image.width, image.height, BLACK);
    Texture2D texture = LoadTextureFromImage(image2);

    UnloadImage(image2);

  

    float lightYaw = 0;
    float lightPitch = 0;

    while (!WindowShouldClose())
    {
        if(IsKeyDown(KEY_LEFT)){
            lightYaw -= 1.5 * GetFrameTime();
        }
        if(IsKeyDown(KEY_RIGHT)){
            lightYaw += 1.5 * GetFrameTime();
        }
        if(IsKeyDown(KEY_DOWN)){
            lightPitch -= 1.5 * GetFrameTime();
        }
        if(IsKeyDown(KEY_UP)){
            lightPitch += 1.5 * GetFrameTime();
        }
        if(IsKeyPressed(KEY_S)){
            image.write("output.tga");
        }
        lightDir = {std::cos(lightPitch) * std::sin(lightYaw), std::sin(lightPitch), std::cos(lightPitch) * std::cos(lightYaw)};
        std::fill(image.pixels.begin(), image.pixels.end(), tr::Pixel{0, 0, 0});
        std::fill(zBuf.begin(), zBuf.end(), -std::numeric_limits<float>::max());

        tr::drawModel(model, image, {255, 255, 255}, zBuf, lightDir);
        tr::drawModel(model1, image, {255, 255, 255}, zBuf, lightDir);
        tr::drawModel(model2, image, {255, 255, 255}, zBuf, lightDir);

        toRaylibPixels(image, screenPixels);
        UpdateTexture(texture, screenPixels.data());

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
}