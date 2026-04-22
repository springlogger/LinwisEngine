#include <lw/scene/MeshGeometry.h>
#include <lw/math/Vector3.h>

namespace lw {

MeshGeometry createCubeGeometry(float width, float height, float depth)
{
    MeshGeometry geometry;

    const float hx = width * 0.5f;
    const float hy = height * 0.5f;
    const float hz = depth * 0.5f;

    geometry.vertices =
    {
        // Front
        {{-hx, -hy,  hz}}, // 0
        {{ hx, -hy,  hz}}, // 1
        {{-hx,  hy,  hz}}, // 2
        {{ hx,  hy,  hz}}, // 3

        // Back
        {{ hx, -hy, -hz}}, // 4
        {{-hx, -hy, -hz}}, // 5
        {{ hx,  hy, -hz}}, // 6
        {{-hx,  hy, -hz}}, // 7

        // Left
        {{-hx, -hy, -hz}}, // 8
        {{-hx, -hy,  hz}}, // 9
        {{-hx,  hy, -hz}}, // 10
        {{-hx,  hy,  hz}}, // 11

        // Right
        {{ hx, -hy,  hz}}, // 12
        {{ hx, -hy, -hz}}, // 13
        {{ hx,  hy,  hz}}, // 14
        {{ hx,  hy, -hz}}, // 15

        // Top
        {{-hx,  hy,  hz}}, // 16
        {{ hx,  hy,  hz}}, // 17
        {{-hx,  hy, -hz}}, // 18
        {{ hx,  hy, -hz}}, // 19

        // Bottom
        {{-hx, -hy, -hz}}, // 20
        {{ hx, -hy, -hz}}, // 21
        {{-hx, -hy,  hz}}, // 22
        {{ hx, -hy,  hz}}  // 23
    };

    geometry.indices =
    {
        // Front
        0, 1, 2,
        2, 1, 3,

        // Back
        4, 5, 6,
        6, 5, 7,

        // Left
        8, 9, 10,
        10, 9, 11,

        // Right
        12, 13, 14,
        14, 13, 15,

        // Top
        16, 17, 18,
        18, 17, 19,

        // Bottom
        20, 21, 22,
        22, 21, 23
    };

    return geometry;
}

} // namespace lw
