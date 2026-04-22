#pragma once

#include <lw/math/Matrix4.h>
#include <lw/math/Vector3.h>
#include <lw/math/Quaternion.h>

#include <vector>

namespace lw {

class Object3D {
public:
    Vector3 position;
    Quaternion quaternion;
    Vector3 scale;
    Matrix4 objectMatrix;

    Object3D();
    virtual ~Object3D() = default;

    void updateMatrix();
    const Matrix4& getMatrix() const { return objectMatrix; };

    void setPosition(const Vector3& position);
    void setRotation(const Quaternion& newQuaternion);
    void setRotation(const Vector3& rotation);
    void setScale(const Vector3& scale);

    const Vector3& getPosition() const;
    const Quaternion& getRotation() const;
    const Vector3& getScale() const;
};

} // namespace lw
