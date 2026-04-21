#pragma once

#include <lw/math/Vector3.h>

namespace lw::math {

constexpr float pi = 3.14159265358979323846f;

constexpr float degToRad(float degrees) {
    return degrees * pi / 180.0f;
}

float pointToPlaneDistance(const Vector3& point, const Vector3& pointOnPlane, const Vector3& planeNormal);

} // namespace lw::math
