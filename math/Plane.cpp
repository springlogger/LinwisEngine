#include "Plane.h"

#include <cmath>

namespace lw {

Plane::Plane(const Vector3& normal, const Vector3& point)
    : distance(0.0f),
      normal(normal.normalized()) {
    distance = this->normal.dot(point);
}

Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
    : distance(0.0f),
      normal((point2 - point1).cross(point3 - point1).normalized()) {
    if (normal.isNearZero()) {
        normal = Vector3(0.0f, 1.0f, 0.0f);
    }
    distance = normal.dot(point1);
}

float Plane::distanceToPoint(const Vector3& point) const {
    return std::abs(normal.dot(point) - distance);
}

const Vector3& Plane::getNormal() const {
    return normal;
}

float Plane::getDist() const {
    return distance;
}

} // namespace lw
