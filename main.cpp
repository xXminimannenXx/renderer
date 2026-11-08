#include <raylib.h>
#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>
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
    tr::Vector3f scaledForward =  v3f_scale(cam.forward(),1.5*GetFrameTime());
    tr::Vector3f scaledRight =  v3f_scale(cam.right(),1.5*GetFrameTime());
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
       cam.pos =   tr::v3f_Sub(cam.pos,scaledRight);
    }
    if (IsKeyDown(KEY_D))
    {
        cam.pos = tr::v3f_add(scaledRight, cam.pos);
    }
    if (IsKeyDown(KEY_S))
    {
        cam.pos = v3f_Sub(cam.pos,scaledForward);
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
    cam.pitch = std::clamp(cam.pitch,-1.5f,1.5f);

}
int main()
{
    tr::Vector3f lightDir = {0, 0, 1};

    tr::Image image(800, 800);
    tr::Image tex(0,0);
    tex.read("obj/african_head/african_head_diffuse.tga");
    tex.write("outputTex.tga");

    tr::Model model("obj/african_head/african_head.obj");
    tr::Model model1("obj/african_head/african_head_eye_outer.obj");
    tr::Model model2("obj/african_head/african_head_eye_inner.obj");

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

        tr::drawModel(model, image, {255, 255, 255}, zBuf, lightDir, modelYaw, modelPitch, cam);
        tr::drawModel(model1, image, {255, 255, 255}, zBuf, lightDir, modelYaw, modelPitch, cam);
        tr::drawModel(model2, image, {255, 255, 255}, zBuf, lightDir, modelYaw, modelPitch, cam);

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