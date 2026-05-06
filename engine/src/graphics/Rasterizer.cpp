#include <lw/graphics/Rasterizer.h>

#include <cmath>
#include <algorithm>

namespace lw
{

static float SrgbToLinear(float c) {
    return c <= 0.04045f ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
}

uint32_t SampleTexture(const Texture& texture, float u, float v) {
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);

    int x = static_cast<int>(u * (texture.getWidth()  - 1));
    int y = static_cast<int>(v * (texture.getHeight() - 1));

    // stb_image RGBA в памяти → little-endian uint32_t: биты [7:0]=R [15:8]=G [23:16]=B [31:24]=A
    const uint32_t packed = texture.getPixels()[y * texture.getWidth() + x];
    float r = ((packed >>  0) & 0xFF) / 255.0f;
    float g = ((packed >>  8) & 0xFF) / 255.0f;
    float b = ((packed >> 16) & 0xFF) / 255.0f;

    if (true) {
        r = SrgbToLinear(r);
        g = SrgbToLinear(g);
        b = SrgbToLinear(b);
    }

    // Windows GDI BI_RGB 32-bit: в памяти [B, G, R, X] → uint32_t: 0x00RRGGBB
    const auto u8 = [](float f) {
        return static_cast<uint32_t>(std::clamp(f, 0.0f, 1.0f) * 255.0f + 0.5f) & 0xFF;
    };
    return (u8(r) << 16) | (u8(g) << 8) | u8(b);
}

// векторное AB * AP
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
    const Texture& texture,
    uint32_t color)
{
    const ScreenVertex& v0 = triangle.v0;
    const ScreenVertex& v1 = triangle.v1;
    const ScreenVertex& v2 = triangle.v2;

    const float area = EdgeFunction(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
    // Y-flip in viewport inverts winding: CCW models become CW on screen (area < 0)
    if (std::abs(area) < 1e-6f || area > 0.0f) {
        //backface culling
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
            // берём именно центр пикселя (по конвенции)
            const float px = static_cast<float>(x) + 0.5f;
            const float py = static_cast<float>(y) + 0.5f;

            // обычно модели хранят именно CCW порядок вершин
            const float w0 = EdgeFunction(v1.x, v1.y, v2.x, v2.y, px, py);
            const float w1 = EdgeFunction(v2.x, v2.y, v0.x, v0.y, px, py);
            const float w2 = EdgeFunction(v0.x, v0.y, v1.x, v1.y, px, py);

            if (w0 > 0.0f || w1 > 0.0f || w2 > 0.0f) {
                continue;
            }

            const float alpha = w0 / area;
            const float beta  = w1 / area;
            const float gamma = w2 / area;

            const float depth =
                alpha * v0.z +
                beta  * v1.z +
                gamma * v2.z;

            const float interpInvW =
                alpha * v0.invW + beta * v1.invW + gamma * v2.invW;
            const float u =
                (alpha * v0.uOverW + beta * v1.uOverW + gamma * v2.uOverW) / interpInvW;
            const float v =
                (alpha * v0.vOverW + beta * v1.vOverW + gamma * v2.vOverW) / interpInvW;

            const uint32_t text_color = SampleTexture(texture, u, v);

            if (depth < renderTarget.depth.At(x, y))
            {
                renderTarget.depth.At(x, y) = depth;
                if (text_color) {
                    renderTarget.color.PutPixel(x, y, text_color);
                } else {
                    renderTarget.color.PutPixel(x, y, color);
                }
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
    const Texture& texture,
    uint32_t color)
{
    for (const ScreenTriangle& triangle : triangles)
    {
        DrawFilledTriangle(triangle, renderTarget, texture, color);
    }
}

} // namespace lw
