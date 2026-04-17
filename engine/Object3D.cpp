#include "Object3D.h"

namespace lw {

Object3D::Object3D()
    : position(0.0f, 0.0f, 0.0f),
      rotation(0.0f, 0.0f, 0.0f),
      scale(1.0f, 1.0f, 1.0f),
      objectMatrix() {
}

void Object3D::updateMatrix() {
    objectMatrix.identity()
        .translate(position)
        .rotateZ(rotation.z)
        .rotateY(rotation.y)
        .rotateX(rotation.x)
        .scale(scale);
}

void Object3D::setPosition(const Vector3& newPosition) {
    position = newPosition;
}

void Object3D::setRotation(const Vector3& newRotation) {
    rotation = newRotation;
}

void Object3D::setScale(const Vector3& newScale) {
    scale = newScale;
}

const Vector3& Object3D::getPosition() const {
    return position;
}

const Vector3& Object3D::getRotation() const {
    return rotation;
}

const Vector3& Object3D::getScale() const {
    return scale;
}

} // namespace lw
