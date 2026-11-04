#pragma once
#include <string>
#include <vector>
#include "Geometry.h"

namespace tr
{

    class Model
    {
    public:
        std::vector<Vector3f> verts;
        std::vector<int> trig;
        std::vector<Vector3f> norms;
        std::vector<int> normIdx;

        Model(const std::string &filePath);

    private:
        void read(const std::string &filePath);
    };

}