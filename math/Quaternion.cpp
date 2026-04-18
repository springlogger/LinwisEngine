#include "Quaternion.h"

#include <cmath>

namespace lw {

Quaternion::Quaternion()
    : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {
}

Quaternion::Quaternion(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
        w * other.w - x * other.x - y * other.y - z * other.z
    );
}

Quaternion Quaternion::conjugated() const{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::identity() {
    return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

float Quaternion::length() const {
    return std::sqrt(x*x + y*y + z*z + w*w);
}

Quaternion& Quaternion::normalize() {
    const float len = length();

    if (len == 0.0f) {
        *this = identity();
        return *this;
    }

    x= x/len;
    y = y/len;
    z = z/len;
    w = w/len;

    return *this;
}

Quaternion Quaternion::normalized() const
{
    const float len = length();
    if (len == 0.0f) {
        return identity();
    }

    return Quaternion(x / len, y / len, z / len, w / len);
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, float angleRadians) {

    Quaternion q;

    const Vector3 normalizedAxis = axis.normalized();
    const float halfAngle = angleRadians * 0.5f;
    const float sinHalfAngle = std::sin(halfAngle);

    q.x = normalizedAxis.x * sinHalfAngle;
    q.y = normalizedAxis.y * sinHalfAngle;
    q.z = normalizedAxis.z * sinHalfAngle;
    q.w = std::cos(halfAngle);

    return q.normalize();
}

Quaternion Quaternion::FromEuler(float pitch, float yaw, float roll)
{
    Quaternion qx = Quaternion::FromAxisAngle(Vector3(1, 0, 0), pitch);
    Quaternion qy = Quaternion::FromAxisAngle(Vector3(0, 1, 0), yaw);
    Quaternion qz = Quaternion::FromAxisAngle(Vector3(0, 0, 1), roll);

    Quaternion q = qx * qy * qz;

    return q.normalized();
}

Vector3 Quaternion::rotate(const Vector3& vector) const {
    Quaternion q = normalized();
    Quaternion p(vector.x, vector.y, vector.z, 0.0f);
    Quaternion result = q * p * q.conjugated();

    return Vector3(result.x, result.y, result.z);
}

} // namespace lw
