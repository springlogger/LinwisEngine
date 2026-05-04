#pragma once

#include <lw/graphics/GraphicsTypes.h>
#include <lw/graphics/RenderTarget.h>

#include <vector>
#include <cstdint>

namespace lw
{

void RasterizeTriangles(
    RenderTarget& renderTarget,
    const std::vector<ScreenTriangle>& triangles,
    uint32_t color);

void RasterizeWireframe(
    RenderTarget& renderTarget,
    const std::vector<ScreenTriangle>& triangles,
    uint32_t color);

void RasterizeLines(
    RenderTarget& renderTarget,
    const std::vector<ScreenSegment>& segments);

} // namespace lw
