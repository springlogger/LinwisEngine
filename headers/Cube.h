#pragma once

#include "Object3D.h"

#include <vector>

namespace lw {

class Cube : public Object3D {
public:
    Cube();
    Cube(const Vector3& position, const Vector3& rotation, const Vector3& scale);

    const std::vector<Vector3>& getVertices() const override;
    const std::vector<Edge>& getEdges() const override;

private:
    std::vector<Vector3> vertices;
    std::vector<Edge> edges;
};

} // namespace lw
