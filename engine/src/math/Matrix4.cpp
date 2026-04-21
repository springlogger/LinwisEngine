#include <lw/math/Matrix4.h>

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

// v' = q * v * q^(-1)
// для нормализованного кватерниона q^(-1) = conjugate(q)
Matrix4 Matrix4::fromQuaternion(const Quaternion& q) {
    Quaternion n = q.normalized();

    const float x = n.x;
    const float y = n.y;
    const float z = n.z;
    const float w = n.w;

    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;
    const float xy = x * y;
    const float xz = x * z;
    const float yz = y * z;
    const float wx = w * x;
    const float wy = w * y;
    const float wz = w * z;

    Matrix4 matrix;
    matrix.identity();

    matrix[0]  = 1.0f - 2.0f * (yy + zz);
    matrix[1]  = 2.0f * (xy - wz);
    matrix[2]  = 2.0f * (xz + wy);

    matrix[4]  = 2.0f * (xy + wz);
    matrix[5]  = 1.0f - 2.0f * (xx + zz);
    matrix[6]  = 2.0f * (yz - wx);

    matrix[8]  = 2.0f * (xz - wy);
    matrix[9]  = 2.0f * (yz + wx);
    matrix[10] = 1.0f - 2.0f * (xx + yy);

    return matrix;
}

Matrix4& Matrix4::rotate(const Quaternion& q) {
    return *this *= fromQuaternion(q);
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

Matrix4 Matrix4::rotationX(float angleRad) {
    Matrix4 matrix;
    const float c = std::cos(angleRad);
    const float s = std::sin(angleRad);

    matrix[5] = c;
    matrix[6] = -s;
    matrix[9] = s;
    matrix[10] = c;
    return matrix;
}

Matrix4 Matrix4::rotationY(float angleRad) {
    Matrix4 matrix;
    const float c = std::cos(angleRad);
    const float s = std::sin(angleRad);

    matrix[0] = c;
    matrix[2] = s;
    matrix[8] = -s;
    matrix[10] = c;
    return matrix;
}

Matrix4 Matrix4::rotationZ(float angleRad) {
    Matrix4 matrix;
    const float c = std::cos(angleRad);
    const float s = std::sin(angleRad);

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

Matrix4& Matrix4::rotateX(float angleRad) {
    return *this *= rotationX(angleRad);
}

Matrix4& Matrix4::rotateY(float angleRad) {
    return *this *= rotationY(angleRad);
}

Matrix4& Matrix4::rotateZ(float angleRad) {
    return *this *= rotationZ(angleRad);
}

} // namespace lw
