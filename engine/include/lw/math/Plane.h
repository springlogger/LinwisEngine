#pragma once

#include <lw/math/Vector3.h>

namespace lw {

class Plane {
public:
    Plane(const Vector3& normal, const Vector3& point);
    Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3);

    float distanceToPoint(const Vector3& point) const;

    const Vector3& getNormal() const;
    float getDist() const;

private:
    float distance;
    Vector3 normal;
};

} // namespace lw
