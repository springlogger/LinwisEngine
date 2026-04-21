#pragma once

#include <lw/math/Vector3.h>

#include <vector>
#include <stdint.h>

namespace lw
{

struct Vertex
{
    Vector3 position;
    // потом добавишь normal, uv, color
};

class Geometry {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

}