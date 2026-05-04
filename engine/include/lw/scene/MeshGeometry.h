#pragma once

#include <lw/math/Vector3.h>

#include <vector>
#include <stdint.h>

namespace lw
{

struct MeshVertex
{
    Vector3 position;
    uint32_t color = 0;
    // потом добавишь normal, uv
};

class MeshGeometry {
public:
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
};

}