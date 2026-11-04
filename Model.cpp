#include "Model.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace tr
{

    Model::Model(const std::string &filePath)
    {
        read(filePath);
    }

    void Model::read(const std::string &filePath)
    {
        std::ifstream fi(filePath);
        if (!fi.is_open())
        {
            std::cerr << "file cant open\n";
            return;
        }
        std::string line;
        while (std::getline(fi, line))
        {
            std::istringstream si(line);
            std::string word;
            si >> word;
            if (word == "v")
            {
                Vector3f v;
                si >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
            if (word == "vn")
            {
                Vector3f v;
                si >> v.x >> v.y >> v.z;
                norms.push_back(v);
            }
            if (word == "f")
            {
                for (int i = 0; i < 3; i++)
                {
                    std::string group;
                    si >> group;
                    std::replace(group.begin(), group.end(), '/', ' ');
                    int pos;
                    int texCord;
                    int norm;
                    std::istringstream cS(group);
                    cS >> pos >> texCord >> norm;
                    trig.push_back(pos - 1);
                    normIdx.push_back(norm - 1);
                }
            }
        }
    }

}