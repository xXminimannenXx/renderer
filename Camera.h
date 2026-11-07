#pragma once
#include "Geometry.h"

namespace tr
{
    class Camera
    {

    public:
        tr::Vector3f pos;
        float focalLength = 3;
        float yaw = 0;
        float pitch = 0;
       Camera(Vector3f pos)
        {
            this->pos = pos;
        };

        tr::Vector3f project(tr::Vector3f a) const;
        tr::Vector3f directionTo(tr::Vector3f a) const;
        tr::Vector3f toView(tr::Vector3f a) const;
        tr::Vector3f forward() const; 
        tr::Vector3f right() const; 
    };

}