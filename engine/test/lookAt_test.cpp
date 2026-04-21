#include "TestHelpers.h"

#include "Camera.h"
#include "Math.h"
#include "Vector3.h"

namespace lw::test {

void runCameraTests(TestStats& stats) {
    const Camera camera(
        Vector3(0.0f, 0.0f, 5.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        math::degToRad(60.0f),
        1.0f,
        0.1f,
        100.0f
    );

    const Vector3 originInView = camera.getViewMatrix().transformPoint(Vector3(0.0f, 0.0f, 0.0f));
    stats.require("Camera looks at target", almostEqual(originInView.x, 0.0f) && almostEqual(originInView.y, 0.0f) && almostEqual(originInView.z, -5.0f));

    const Vector3 right(camera.getViewMatrix()[0], camera.getViewMatrix()[1], camera.getViewMatrix()[2]);
    const Vector3 up(camera.getViewMatrix()[4], camera.getViewMatrix()[5], camera.getViewMatrix()[6]);
    stats.require("Camera basis is orthogonal", almostEqual(right.dot(up), 0.0f));
}

} // namespace lw::test
