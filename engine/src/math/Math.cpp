#include <lw/math/Math.h>
#include <lw/math/Vector3.h>

namespace lw::math {

float pointToPlaneDistance(const Vector3& point, const Vector3& pointOnPlane, const Vector3& planeNormal) {
    return (point - pointOnPlane).dot(planeNormal.normalized());
}

} // namespace lw::math
