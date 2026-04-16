#include "Matrix4.h"

#include <cmath>

namespace lw {

Matrix4::Matrix4() {
    identity();
}

float& Matrix4::operator[](std::size_t index) {
    return values[index];
}

const float& Matrix4::operator[](std::size_t index) const {
    return values[index];
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;

    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            float sum = 0.0f;
            for (std::size_t k = 0; k < 4; ++k) {
                sum += values[row * 4 + k] * other.values[k * 4 + col];
            }
            result.values[row * 4 + col] = sum;
        }
    }

    return result;
}

Matrix4& Matrix4::operator*=(const Matrix4& other) {
    *this = *this * other;
    return *this;
}

Matrix4& Matrix4::identity() {
    values = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return *this;
}

Vector3 Matrix4::transformPoint(const Vector3& vector) const {
    return Vector3(
        values[0] * vector.x + values[1] * vector.y + values[2] * vector.z + values[3],
        values[4] * vector.x + values[5] * vector.y + values[6] * vector.z + values[7],
        values[8] * vector.x + values[9] * vector.y + values[10] * vector.z + values[11]
    );
}

Vector3 Matrix4::transformVector(const Vector3& vector) const {
    return Vector3(
        values[0] * vector.x + values[1] * vector.y + values[2] * vector.z,
        values[4] * vector.x + values[5] * vector.y + values[6] * vector.z,
        values[8] * vector.x + values[9] * vector.y + values[10] * vector.z
    );
}

Vector4 Matrix4::transform(const Vector4& vector) const {
    return Vector4(
        values[0] * vector.x + values[1] * vector.y + values[2] * vector.z + values[3] * vector.w,
        values[4] * vector.x + values[5] * vector.y + values[6] * vector.z + values[7] * vector.w,
        values[8] * vector.x + values[9] * vector.y + values[10] * vector.z + values[11] * vector.w,
        values[12] * vector.x + values[13] * vector.y + values[14] * vector.z + values[15] * vector.w
    );
}

Matrix4 Matrix4::translation(const Vector3& position) {
    Matrix4 matrix;
    matrix[3] = position.x;
    matrix[7] = position.y;
    matrix[11] = position.z;
    return matrix;
}

Matrix4 Matrix4::scaling(const Vector3& scale) {
    Matrix4 matrix;
    matrix[0] = scale.x;
    matrix[5] = scale.y;
    matrix[10] = scale.z;
    return matrix;
}

Matrix4 Matrix4::rotationX(float angle) {
    Matrix4 matrix;
    const float c = std::cos(angle);
    const float s = std::sin(angle);

    matrix[5] = c;
    matrix[6] = -s;
    matrix[9] = s;
    matrix[10] = c;
    return matrix;
}

Matrix4 Matrix4::rotationY(float angle) {
    Matrix4 matrix;
    const float c = std::cos(angle);
    const float s = std::sin(angle);

    matrix[0] = c;
    matrix[2] = s;
    matrix[8] = -s;
    matrix[10] = c;
    return matrix;
}

Matrix4 Matrix4::rotationZ(float angle) {
    Matrix4 matrix;
    const float c = std::cos(angle);
    const float s = std::sin(angle);

    matrix[0] = c;
    matrix[1] = -s;
    matrix[4] = s;
    matrix[5] = c;
    return matrix;
}

Matrix4& Matrix4::translate(const Vector3& position) {
    return *this *= translation(position);
}

Matrix4& Matrix4::scale(const Vector3& scale) {
    return *this *= scaling(scale);
}

Matrix4& Matrix4::rotateX(float angle) {
    return *this *= rotationX(angle);
}

Matrix4& Matrix4::rotateY(float angle) {
    return *this *= rotationY(angle);
}

Matrix4& Matrix4::rotateZ(float angle) {
    return *this *= rotationZ(angle);
}

} // namespace lw
