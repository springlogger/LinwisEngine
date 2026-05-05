#pragma once

#include <lw/math/Vector3.h>
#include <lw/math/Vector2.h>

#include <vector>
#include <stdint.h>

namespace lw
{

struct MeshVertex
{
    Vector3 position;
    uint32_t color = 0;
    Vector2 uv = Vector2(0.0f, 0.0f);
    Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);

    // потом добавишь normal, uv
};

class MeshGeometry {
public:
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;

    std::vector<uint32_t> index_buffer;
    std::vector<MeshVertex> vertex_buffer;
};

}