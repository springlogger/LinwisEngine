#pragma once

#include "Matrix4.h"
#include "Vector3.h"

#include <utility>
#include <vector>

namespace lw {

using Edge = std::pair<int, int>;

class Object3D {
public:
    Object3D();
    virtual ~Object3D() = default;

    void setPosition(const Vector3& position);
    void setRotation(const Vector3& rotation);
    void setScale(const Vector3& scale);

    const Vector3& getPosition() const;
    const Vector3& getRotation() const;
    const Vector3& getScale() const;

    void updateMatrix();
    const Matrix4& getMatrix() const;

    virtual const std::vector<Vector3>& getVertices() const;
    virtual const std::vector<Edge>& getEdges() const;
    bool hasGeometry() const;

protected:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Matrix4 matrix;
};

} // namespace lw
