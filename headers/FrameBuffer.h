#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>

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