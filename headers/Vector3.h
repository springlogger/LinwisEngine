#pragma once

#include <iosfwd>

namespace lw {

class Vector3 {
public:
    float x;
    float y;
    float z;

    constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(float scalar) const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    Vector3& operator+=(float scalar);
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    bool isNearZero(float epsilon = 1e-6f) const;

    float dot(const Vector3& other) const;
    float lengthSquared() const;
    float length() const;
    Vector3 cross(const Vector3& other) const;

    void normalize();
    Vector3 normalized() const;

    Vector3 clone() const;

    Vector3 add(const Vector3& other) const;
    Vector3 sub(const Vector3& other) const;
    Vector3 multiplyScalar(float scalar) const;

    Vector3 addScalar(const float scalar) const;

    void print() const;
};

Vector3 operator*(float scalar, const Vector3& vector);
std::ostream& operator<<(std::ostream& stream, const Vector3& vector);

} // namespace lw
