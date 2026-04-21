#include "TestHelpers.h"

#include "Plane.h"
#include "Vector3.h"

#include <cmath>

namespace lw::test {

void runVector3Tests(TestStats& stats) {
    const Vector3 a(1.0f, 2.0f, 3.0f);
    const Vector3 b(4.0f, 5.0f, 6.0f);

    const Vector3 sum = a + b;
    stats.require("Vector3 addition", almostEqual(sum.x, 5.0f) && almostEqual(sum.y, 7.0f) && almostEqual(sum.z, 9.0f));

    const Vector3 diff = b - a;
    stats.require("Vector3 subtraction", almostEqual(diff.x, 3.0f) && almostEqual(diff.y, 3.0f) && almostEqual(diff.z, 3.0f));

    stats.require("Vector3 dot product", almostEqual(a.dot(b), 32.0f));

    const Vector3 cross = Vector3(1.0f, 0.0f, 0.0f).cross(Vector3(0.0f, 1.0f, 0.0f));
    stats.require("Vector3 cross product", almostEqual(cross.x, 0.0f) && almostEqual(cross.y, 0.0f) && almostEqual(cross.z, 1.0f));

    const Vector3 normalized = Vector3(3.0f, 0.0f, 0.0f).normalized();
    stats.require("Vector3 normalization", almostEqual(normalized.length(), 1.0f));

    const Plane plane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 5.0f));
    stats.require("Plane distance", almostEqual(plane.distanceToPoint(Vector3(10.0f, 20.0f, 10.0f)), 5.0f));
}

} // namespace lw::test
