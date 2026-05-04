#pragma once

namespace lw {

class Vector4 {
public:
    float x;
    float y;
    float z;
    float w;

    constexpr Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    constexpr Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(float scalar) const;
    Vector4 operator+(const Vector4& other) const;
};

} // namespace lw
