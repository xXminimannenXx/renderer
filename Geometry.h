#pragma once
#include <cmath>

namespace tr
{

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
    inline Vector3f v3f_Sub(Vector3f a, Vector3f b)
    {

        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }
    inline Vector3f v3f_cross(Vector3f a, Vector3f b)
    {

        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
    inline float v3f_dot(Vector3f a, Vector3f b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    inline Vector3f v3f_norm(Vector3f a)
    {
        float leng = std::sqrt(v3f_dot(a, a));
        if (leng == 0)
            return a;
        return {a.x / leng, a.y / leng, a.z / leng};
    }
    inline Vector3f v3f_add(Vector3f a, Vector3f b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }
    inline Vector3f v3f_scale(Vector3f a, float s)
    {
        return {a.x * s, a.y * s, a.z * s};
    }

}