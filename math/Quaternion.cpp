#include "Quaternion.h"

#include <cmath>

namespace lw {

Quaternion::Quaternion()
    : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {
}

Quaternion::Quaternion(const Vector3& axis, float angle) {
    const Vector3 normalizedAxis = axis.normalized();
    const float halfAngle = angle * 0.5f;
    const float sinHalfAngle = std::sin(halfAngle);

    x = normalizedAxis.x * sinHalfAngle;
    y = normalizedAxis.y * sinHalfAngle;
    z = normalizedAxis.z * sinHalfAngle;
    w = std::cos(halfAngle);
}

Vector3 Quaternion::rotate(const Vector3& vector) const {
    const Vector3 qVector(x, y, z);
    const Vector3 t = 2.0f * qVector.cross(vector);
    return vector + (w * t) + qVector.cross(t);
}

} // namespace lw
