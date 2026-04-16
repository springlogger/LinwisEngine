#include "Math.h"

namespace lw::math {

float pointToPlaneDistance(const Vector3& point, const Vector3& pointOnPlane, const Vector3& planeNormal) {
    return (point - pointOnPlane).dot(planeNormal.normalized());
}

} // namespace lw::math
