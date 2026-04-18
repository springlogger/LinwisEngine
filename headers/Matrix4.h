#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

#include <array>
#include <cstddef>

namespace lw {

class Matrix4 {
public:
    Matrix4();

    float& operator[](std::size_t index);
    const float& operator[](std::size_t index) const;

    Matrix4 operator*(const Matrix4& other) const;
    Matrix4& operator*=(const Matrix4& other);

    Matrix4& identity();

    Matrix4& translate(const Vector3& position);
    Matrix4& rotate(const Quaternion& q);
    Matrix4& rotateX(float angleRad);
    Matrix4& rotateY(float angleRad);
    Matrix4& rotateZ(float angleRad);
    Matrix4& scale(const Vector3& scale);

    Vector3 transformPoint(const Vector3& vector) const;
    Vector3 transformVector(const Vector3& vector) const;
    Vector4 transform(const Vector4& vector) const;

    static Matrix4 translation(const Vector3& position);
    static Matrix4 fromQuaternion(const Quaternion& quaternion);
    static Matrix4 rotationX(float angleRad);
    static Matrix4 rotationY(float angleRad);
    static Matrix4 rotationZ(float angleRad);
    static Matrix4 scaling(const Vector3& scale);

private:
    std::array<float, 16> values;
};

} // namespace lw
