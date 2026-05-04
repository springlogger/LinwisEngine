#include <lw/graphics/Rasterizer.h>

#include <cmath>
#include <algorithm>

namespace lw
{

static float EdgeFunction(
    float ax, float ay,
    float bx, float by,
    float px, float py)
{
    return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
}

static void DrawFilledTriangle(
    const ScreenTriangle& triangle,
    RenderTarget& renderTarget,
    uint32_t color)
{
    const ScreenVertex& v0 = triangle.v0;
    const ScreenVertex& v1 = triangle.v1;
    const ScreenVertex& v2 = triangle.v2;

    const float area = EdgeFunction(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
    if (std::abs(area) < 1e-6f || area < 0.0f) {
        return;
    }

    const int minX = std::max(
        0,
        static_cast<int>(std::floor(std::min({v0.x, v1.x, v2.x})))
    );
    const int maxX = std::min(
        renderTarget.color.width - 1,
        static_cast<int>(std::ceil(std::max({v0.x, v1.x, v2.x})))
    );
    const int minY = std::max(
        0,
        static_cast<int>(std::floor(std::min({v0.y, v1.y, v2.y})))
    );
    const int maxY = std::min(
        renderTarget.color.height - 1,
        static_cast<int>(std::ceil(std::max({v0.y, v1.y, v2.y})))
    );

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            const float px = static_cast<float>(x) + 0.5f;
            const float py = static_cast<float>(y) + 0.5f;

            const float w0 = EdgeFunction(v1.x, v1.y, v2.x, v2.y, px, py);
            const float w1 = EdgeFunction(v2.x, v2.y, v0.x, v0.y, px, py);
            const float w2 = EdgeFunction(v0.x, v0.y, v1.x, v1.y, px, py);

            if (w0 < 0.0f || w1 < 0.0f || w2 < 0.0f) {
                continue;
            }

            const float alpha = w0 / area;
            const float beta  = w1 / area;
            const float gamma = w2 / area;

            const float depth =
                alpha * v0.z +
                beta  * v1.z +
                gamma * v2.z;

            if (depth < renderTarget.depth.At(x, y))
            {
                renderTarget.depth.At(x, y) = depth;
                renderTarget.color.PutPixel(x, y, color);
            }
        }
    }
}

static void DrawLine(
    RenderTarget& renderTarget,
    int x0, int y0,
    int x1, int y1,
    uint32_t color)
{
    const int dx = std::abs(x1 - x0);
    const int dy = std::abs(y1 - y0);
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;
    int error = dx - dy;

    while (true)
    {
        renderTarget.color.PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;

        const int e2 = 2 * error;
        if (e2 > -dy) { error -= dy; x0 += sx; }
        if (e2 <  dx) { error += dx; y0 += sy; }
    }
}

void RasterizeLines(
    RenderTarget& renderTarget,
    const std::vector<ScreenSegment>& segments)
{
    for (const ScreenSegment& seg : segments)
    {
        DrawLine(
            renderTarget,
            static_cast<int>(seg.a.x),
            static_cast<int>(seg.a.y),
            static_cast<int>(seg.b.x),
            static_cast<int>(seg.b.y),
            seg.color
        );
    }
}

void RasterizeWireframe(
    RenderTarget& renderTarget,
    const std::vector<ScreenTriangle>& triangles,
    uint32_t color)
{
    for (const ScreenTriangle& t : triangles)
    {
        DrawLine(renderTarget,
            static_cast<int>(t.v0.x), static_cast<int>(t.v0.y),
            static_cast<int>(t.v1.x), static_cast<int>(t.v1.y),
            color);
        DrawLine(renderTarget,
            static_cast<int>(t.v1.x), static_cast<int>(t.v1.y),
            static_cast<int>(t.v2.x), static_cast<int>(t.v2.y),
            color);
        DrawLine(renderTarget,
            static_cast<int>(t.v2.x), static_cast<int>(t.v2.y),
            static_cast<int>(t.v0.x), static_cast<int>(t.v0.y),
            color);
    }
}

void RasterizeTriangles(
    RenderTarget& renderTarget,
    const std::vector<ScreenTriangle>& triangles,
    uint32_t color)
{
    for (const ScreenTriangle& triangle : triangles)
    {
        DrawFilledTriangle(triangle, renderTarget, color);
    }
}

} // namespace lw
