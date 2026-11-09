#include <raylib.h>
#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Draw.h"
#include "Image.h"
#include "Model.h"
#include "Display.h"

float lightYaw = 0;
float lightPitch = 0;
float modelYaw = 0;
float modelPitch = 0;
tr::Camera cam({0, 0, 3});

void handleInput(tr::Image &image)
{
    Vector2 mouseDelta = GetMouseDelta();
    tr::Vector3f scaledForward = v3f_scale(cam.forward(), 1.5 * GetFrameTime());
    tr::Vector3f scaledRight = v3f_scale(cam.right(), 1.5 * GetFrameTime());
    if (IsKeyDown(KEY_LEFT))
    {
        lightYaw -= 1.5 * GetFrameTime();
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        lightYaw += 1.5 * GetFrameTime();
    }
    if (IsKeyDown(KEY_DOWN))
    {
        lightPitch -= 1.5 * GetFrameTime();
    }
    if (IsKeyDown(KEY_UP))
    {
        lightPitch += 1.5 * GetFrameTime();
    }
    if (IsKeyPressed(KEY_P))
    {
        image.write("output.tga");
    }
    if (IsKeyDown(KEY_A))
    {
        cam.pos = tr::v3f_Sub(cam.pos, scaledRight);
    }
    if (IsKeyDown(KEY_D))
    {
        cam.pos = tr::v3f_add(scaledRight, cam.pos);
    }
    if (IsKeyDown(KEY_S))
    {
        cam.pos = v3f_Sub(cam.pos, scaledForward);
    }
    if (IsKeyDown(KEY_W))
    {
        cam.pos = v3f_add(scaledForward, cam.pos);
    }
    if (IsKeyDown(KEY_Q))
    {

        cam.pos.y -= 1.5 * GetFrameTime();
    }
    if (IsKeyDown(KEY_E))
    {
        cam.pos.y += 1.5 * GetFrameTime();
    }
    cam.yaw -= mouseDelta.x * 0.003;
    cam.pitch -= mouseDelta.y * 0.003;
    cam.pitch = std::clamp(cam.pitch, -1.5f, 1.5f);
}
int main()
{
    tr::Vector3f lightDir = {0, 0, 1};

    tr::Image image(1000, 1000);
    const std::string base = "obj/african_head/african_head";

    std::string headObjPath = base + ".obj";
    std::string headTexPath = base + "_diffuse.tga";
    std::string eyeObjPath = base + "_eye_inner.obj";
    std::string eyeTexPath = base + "_eye_inner_diffuse.tga";

    tr::Model head(headObjPath);
    tr::Model eyes(eyeObjPath);

    tr::Image headTex(0, 0);
    tr::Image eyeTex(0, 0);
    if (!headTex.read(headTexPath))
        std::cerr << "kunde inte läsa " << headTexPath << '\n';
    if (!eyeTex.read(eyeTexPath))
        std::cerr << "kunde inte läsa " << eyeTexPath << '\n';
    std::vector<float> zBuf(image.height * image.width, -std::numeric_limits<float>::max());
    std::vector<Color> screenPixels;

    InitWindow(image.width, image.height, "Renderer");
    DisableCursor();
    SetTargetFPS(60);

    Image image2 = GenImageColor(image.width, image.height, BLACK);
    Texture2D texture = LoadTextureFromImage(image2);
    Color BackgroundColor = BLACK;
    UnloadImage(image2);

    while (!WindowShouldClose())
    {
        handleInput(image);
        lightDir = {std::cos(lightPitch) * std::sin(lightYaw), std::sin(lightPitch), std::cos(lightPitch) * std::cos(lightYaw)};
        std::fill(image.pixels.begin(), image.pixels.end(), tr::Pixel{0, 0, 0});
        std::fill(zBuf.begin(), zBuf.end(), -std::numeric_limits<float>::max());

        tr::drawModel(head, image, {255, 255, 255}, zBuf, lightDir, modelYaw, modelPitch, cam, headTex);
        tr::drawModel(eyes, image, {255, 255, 255}, zBuf, lightDir, modelYaw, modelPitch, cam, eyeTex);

        tr::toRaylibPixels(image, screenPixels);
        UpdateTexture(texture, screenPixels.data());

        BeginDrawing();
        ClearBackground(BackgroundColor);
        DrawTexture(texture, 0, 0, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
}