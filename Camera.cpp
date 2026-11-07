#include "Camera.h"

namespace tr
{

    Vector3f Camera::project(tr::Vector3f a) const
    {
        float f = focalLength/ -a.z;
        return {a.x * f, a.y * f, a.z};
    };
    Vector3f Camera::directionTo(tr::Vector3f a) const
    {

        return {pos.x - a.x, pos.y - a.y, pos.z - a.z};
    };
    Vector3f Camera::toView(Vector3f a) const{
       a = {a.x - pos.x, a.y - pos.y, a.z - pos.z};
       a = tr::v3f_rotateY(a, -yaw);
       a = tr::v3f_rotateX(a, -pitch);
       return a;
    }
    Vector3f Camera::forward() const{
        Vector3f a = {0,0,-1};
        a = tr::v3f_rotateX(a,pitch);
        a = tr::v3f_rotateY(a,yaw);
        return a;
    }
    Vector3f Camera::right() const{
        Vector3f a = {1,0,0};
        return tr::v3f_rotateY(a,yaw);
    }

}