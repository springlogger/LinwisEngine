#pragma once

#include <lw/math/Vector2.h>
#include <lw/math/Vector4.h>

#include <cstdint>

namespace lw
{

struct ClipVertex
{
    Vector4 clipPosition;
    uint32_t color = 0;
    Vector2 uv;
};

struct ClipTriangle
{
    ClipVertex v0;
    ClipVertex v1;
    ClipVertex v2;
};

struct ScreenVertex
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float invW   = 0.0f;
    float uOverW = 0.0f;
    float vOverW = 0.0f;
};

struct ScreenTriangle
{
    ScreenVertex v0;
    ScreenVertex v1;
    ScreenVertex v2;
};

struct ClipSegment
{
    ClipVertex a;
    ClipVertex b;
};

struct ScreenSegment
{
    ScreenVertex a;
    ScreenVertex b;
    uint32_t color;
};

} // namespace lw
