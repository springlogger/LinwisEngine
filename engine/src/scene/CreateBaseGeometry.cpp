#include <lw/scene/MeshGeometry.h>
#include <lw/math/Vector2.h>
#include <lw/math/Vector3.h>

namespace lw {

static MeshVertex CreateMeshVertex(
    const Vector3& position,
    const Vector3& normal,
    const Vector2& uv)
{
    MeshVertex vertex;
    vertex.position = position;
    vertex.normal = normal;
    vertex.uv = uv;
    return vertex;
}

MeshGeometry createCubeGeometry(float width, float height, float depth)
{
    MeshGeometry geometry;

    const float hx = width * 0.5f;
    const float hy = height * 0.5f;
    const float hz = depth * 0.5f;

    const Vector2 uv00(0.0f, 0.0f);
    const Vector2 uv10(1.0f, 0.0f);
    const Vector2 uv01(0.0f, 1.0f);
    const Vector2 uv11(1.0f, 1.0f);

    geometry.vertices =
    {
        // Front (+Z)
        CreateMeshVertex(Vector3(-hx, -hy,  hz), Vector3(0.0f, 0.0f, 1.0f), uv00), // 0
        CreateMeshVertex(Vector3( hx, -hy,  hz), Vector3(0.0f, 0.0f, 1.0f), uv10), // 1
        CreateMeshVertex(Vector3(-hx,  hy,  hz), Vector3(0.0f, 0.0f, 1.0f), uv01), // 2
        CreateMeshVertex(Vector3( hx,  hy,  hz), Vector3(0.0f, 0.0f, 1.0f), uv11), // 3

        // Back (-Z)
        CreateMeshVertex(Vector3( hx, -hy, -hz), Vector3(0.0f, 0.0f, -1.0f), uv00), // 4
        CreateMeshVertex(Vector3(-hx, -hy, -hz), Vector3(0.0f, 0.0f, -1.0f), uv10), // 5
        CreateMeshVertex(Vector3( hx,  hy, -hz), Vector3(0.0f, 0.0f, -1.0f), uv01), // 6
        CreateMeshVertex(Vector3(-hx,  hy, -hz), Vector3(0.0f, 0.0f, -1.0f), uv11), // 7

        // Left (-X)
        CreateMeshVertex(Vector3(-hx, -hy, -hz), Vector3(-1.0f, 0.0f, 0.0f), uv00), // 8
        CreateMeshVertex(Vector3(-hx, -hy,  hz), Vector3(-1.0f, 0.0f, 0.0f), uv10), // 9
        CreateMeshVertex(Vector3(-hx,  hy, -hz), Vector3(-1.0f, 0.0f, 0.0f), uv01), // 10
        CreateMeshVertex(Vector3(-hx,  hy,  hz), Vector3(-1.0f, 0.0f, 0.0f), uv11), // 11

        // Right (+X)
        CreateMeshVertex(Vector3( hx, -hy,  hz), Vector3(1.0f, 0.0f, 0.0f), uv00), // 12
        CreateMeshVertex(Vector3( hx, -hy, -hz), Vector3(1.0f, 0.0f, 0.0f), uv10), // 13
        CreateMeshVertex(Vector3( hx,  hy,  hz), Vector3(1.0f, 0.0f, 0.0f), uv01), // 14
        CreateMeshVertex(Vector3( hx,  hy, -hz), Vector3(1.0f, 0.0f, 0.0f), uv11), // 15

        // Top (+Y)
        CreateMeshVertex(Vector3(-hx,  hy,  hz), Vector3(0.0f, 1.0f, 0.0f), uv00), // 16
        CreateMeshVertex(Vector3( hx,  hy,  hz), Vector3(0.0f, 1.0f, 0.0f), uv10), // 17
        CreateMeshVertex(Vector3(-hx,  hy, -hz), Vector3(0.0f, 1.0f, 0.0f), uv01), // 18
        CreateMeshVertex(Vector3( hx,  hy, -hz), Vector3(0.0f, 1.0f, 0.0f), uv11), // 19

        // Bottom (-Y)
        CreateMeshVertex(Vector3(-hx, -hy, -hz), Vector3(0.0f, -1.0f, 0.0f), uv00), // 20
        CreateMeshVertex(Vector3( hx, -hy, -hz), Vector3(0.0f, -1.0f, 0.0f), uv10), // 21
        CreateMeshVertex(Vector3(-hx, -hy,  hz), Vector3(0.0f, -1.0f, 0.0f), uv01), // 22
        CreateMeshVertex(Vector3( hx, -hy,  hz), Vector3(0.0f, -1.0f, 0.0f), uv11)  // 23
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
