#pragma once

#include "Matrix4.h"
#include "Vector3.h"

#include <vector>

namespace lw {

class Object3D {
public:
    Object3D();
    virtual ~Object3D() = default;

    void updateMatrix();
    const Matrix4& getMatrix() const { return objectMatrix; };

    void setPosition(const Vector3& position);
    void setRotation(const Vector3& rotation);
    void setScale(const Vector3& scale);

    const Vector3& getPosition() const;
    const Vector3& getRotation() const;
    const Vector3& getScale() const;

    virtual bool hasGeometry() const { return false; };

protected:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Matrix4 objectMatrix;
};

} // namespace lw
