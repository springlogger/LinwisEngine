#include "Cube.h"

namespace lw {

Cube::Cube()
    : vertices{
        Vector3(-1.0f, -1.0f, -1.0f),
        Vector3(1.0f, -1.0f, -1.0f),
        Vector3(1.0f, 1.0f, -1.0f),
        Vector3(-1.0f, 1.0f, -1.0f),
        Vector3(-1.0f, -1.0f, 1.0f),
        Vector3(1.0f, -1.0f, 1.0f),
        Vector3(1.0f, 1.0f, 1.0f),
        Vector3(-1.0f, 1.0f, 1.0f)
      },
      edges{
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
      } {
    updateMatrix();
}

Cube::Cube(const Vector3& position, const Vector3& rotation, const Vector3& scale)
    : Cube() {
    setPosition(position);
    setRotation(rotation);
    setScale(scale);
    updateMatrix();
}

const std::vector<Vector3>& Cube::getVertices() const {
    return vertices;
}

const std::vector<Edge>& Cube::getEdges() const {
    return edges;
}

} // namespace lw
