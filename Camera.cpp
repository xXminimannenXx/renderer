#include "Camera.h"

namespace tr
{

    Vector3f Camera::project(tr::Vector3f a) const
    {

        float f = 1 / (1 - a.z / pos.z);
        return {a.x * f, a.y * f, a.z};
    };
    Vector3f Camera::directionTo(tr::Vector3f a) const
    {

        return {pos.x - a.x, pos.y - a.y, pos.z - a.z};
    };
    Vector3f Camera::toView(Vector3f a) const{
        return {a.x - pos.x, a.y - pos.y, a.z - pos.z};
    }

}