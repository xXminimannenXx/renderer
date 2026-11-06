#pragma once
#include "Geometry.h"

namespace tr
{
    class Camera
    {

    public:
        tr::Vector3f pos;

       Camera(Vector3f pos)
        {
            this->pos = pos;
        };

        tr::Vector3f project(tr::Vector3f a) const;
        tr::Vector3f directionTo(tr::Vector3f a) const;
        tr::Vector3f toView(tr::Vector3f a) const;
    };

}