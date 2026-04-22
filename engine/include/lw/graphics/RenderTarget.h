#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>

namespace lw
{

struct DepthBuffer
{
    int width = 0;
    int height = 0;
    std::vector<float> values;

    DepthBuffer() = default;

    DepthBuffer(int w, int h)
        : width(w), height(h), values(w * h, 1.0f)
    {
    }

    void Clear(float depth = 1.0f)
    {
        std::fill(values.begin(), values.end(), depth);
    }

    float& At(int x, int y)
    {
        return values[y * width + x];
    }

    const float& At(int x, int y) const
    {
        return values[y * width + x];
    }
};

struct Framebuffer
{
    int width = 0;
    int height = 0;
    std::vector<uint32_t> pixels;

    Framebuffer() = default;

    Framebuffer(int w, int h)
        : width(w), height(h), pixels(w * h, 0)
    {
    }

    void Clear(uint32_t color)
    {
        std::fill(pixels.begin(), pixels.end(), color);
    }

    void PutPixel(int x, int y, uint32_t color)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;

        pixels[y * width + x] = color;
    }
};

struct RenderTarget
{
    int width = 0;
    int height = 0;

    Framebuffer color;
    DepthBuffer depth;

    RenderTarget() = default;

    RenderTarget(int w, int h)
        : width(w),
          height(h),
          color(w, h),
          depth(w, h)
    {
    }
};

}