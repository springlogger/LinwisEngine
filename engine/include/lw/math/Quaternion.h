#pragma once

#include <lw/math/Vector3.h>

namespace lw {

class Quaternion {
public:

    float x;
    float y;
    float z;
    float w;

    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(const Vector3& axis, float angle);

    Quaternion operator*(const Quaternion& other) const;

    float length() const;
    Quaternion conjugated() const;

    Quaternion& normalize();
    Quaternion normalized() const;

    Vector3 rotate(const Vector3& vector) const;

    static Quaternion FromAxisAngle(const Vector3& axis, float angleRadians);
    static Quaternion FromEuler(float pitch, float yaw, float roll);
    static Quaternion identity();

};

} // namespace lw
