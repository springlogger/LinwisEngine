#include <lw/math/Vector3.h>

#include <cmath>
#include <iostream>

namespace lw {

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator+(float scalar) const {
    return Vector3(x + scalar, y + scalar, z + scalar);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool Vector3::isNearZero(float epsilon) const {
    return lengthSquared() <= epsilon * epsilon;
}

float Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

float Vector3::lengthSquared() const {
    return dot(*this);
}

float Vector3::length() const {
    return std::sqrt(lengthSquared());
}

Vector3 Vector3::cross(const Vector3& other) const {
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

void Vector3::normalize() {
    const float len = length();
    if (len > 1e-6f) {
        *this /= len;
    }
}

Vector3 Vector3::normalized() const {
    const float len = length();
    return len > 1e-6f ? *this / len : Vector3();
}

Vector3 operator*(float scalar, const Vector3& vector) {
    return vector * scalar;
}

std::ostream& operator<<(std::ostream& stream, const Vector3& vector) {
    return stream << "Vector3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}

} // namespace lw
