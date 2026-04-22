#pragma once

#include <lw/math/Vector3.h>

#include <vector>
#include <cstdint>

namespace lw
{
  
  struct LineVertex
{
    Vector3 position;
    uint32_t color;
};

class LineGeometry
{
public:
    std::vector<LineVertex> vertices;
    std::vector<uint32_t> indices;
};
}
