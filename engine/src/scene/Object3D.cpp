#include <lw/scene/Object3D.h>
#include <lw/math/Matrix4.h>

namespace lw {

Object3D::Object3D()
    : position(0.0f, 0.0f, 0.0f),
      quaternion(0.0f, 0.0f, 0.0f, 1.0f),
      scale(1.0f, 1.0f, 1.0f),
      objectMatrix()
{
}

void Object3D::updateMatrix() {
    objectMatrix.identity()
        .translate(position)
        .rotate(quaternion)
        .scale(scale);
}

void Object3D::setPosition(const Vector3& newPosition) {
    position = newPosition;
    updateMatrix();
}

void Object3D::setRotation(const Vector3& rotation) {
    quaternion = Quaternion::FromEuler(rotation.x, rotation.y, rotation.z);
    updateMatrix();
}

void Object3D::setRotation(const Quaternion& newQuaternion) {
    quaternion = newQuaternion.normalized();
    updateMatrix();
}

void Object3D::setScale(const Vector3& newScale) {
    scale = newScale;
    updateMatrix();
}

const Vector3& Object3D::getPosition() const {
    return position;
}

const Quaternion& Object3D::getRotation() const {
    return quaternion;
}

const Vector3& Object3D::getScale() const {
    return scale;
}

} // namespace lw