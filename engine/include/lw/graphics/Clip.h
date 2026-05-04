#pragma once

#include <lw/graphics/GraphicsTypes.h>

#include <vector>

namespace lw
{

[[nodiscard]] std::vector<ClipTriangle> ClipTriangles(const std::vector<ClipTriangle>& triangles);

// Returns false if the segment is fully outside the frustum.
[[nodiscard]] bool ClipLine(ClipVertex& a, ClipVertex& b);

} // namespace lw
