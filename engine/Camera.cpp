#include "Camera.h"

#include "Math.h"

#include <cmath>

namespace lw {

Camera::Camera()
    : Camera(
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        math::degToRad(60.0f),
        1.0f,
        0.1f,
        100.0f
    ) {
}

Camera::Camera(
    const Vector3& eye,
    const Vector3& target,
    const Vector3& up,
    float fovY,
    float aspect,
    float nearPlane,
    float farPlane
)
    : eye(eye),
      target(target),
      up(up.normalized()),
      fovY(fovY),
      aspect(aspect),
      nearPlane(nearPlane),
      farPlane(farPlane) {
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::lookAt(const Vector3& newTarget) {
    target = newTarget;
    updateViewMatrix();
}

void Camera::setAspect(float newAspect) {
    aspect = newAspect;
    updateProjectionMatrix();
}

void Camera::updateViewMatrix() {
    const Vector3 forward = (target - eye).normalized();
    const Vector3 right = forward.cross(up).normalized();
    const Vector3 trueUp = right.cross(forward);

    Matrix4 view;
    view[0] = right.x;      view[1] = right.y;      view[2] = right.z;      view[3] = -right.dot(eye);
    view[4] = trueUp.x;     view[5] = trueUp.y;     view[6] = trueUp.z;     view[7] = -trueUp.dot(eye);
    view[8] = -forward.x;   view[9] = -forward.y;   view[10] = -forward.z;  view[11] = forward.dot(eye);
    view[12] = 0.0f;        view[13] = 0.0f;        view[14] = 0.0f;        view[15] = 1.0f;

    viewMatrix = view;
}

void Camera::updateProjectionMatrix() {
    const float f = 1.0f / std::tan(fovY * 0.5f);

    Matrix4 projection;
    projection[0] = f / aspect;
    projection[5] = f;
    projection[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
    projection[11] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
    projection[14] = -1.0f;
    projection[15] = 0.0f;

    projectionMatrix = projection;
}

void Camera::setPosition(const Vector3& newPosition) {
    eye = newPosition;

    updateViewMatrix();
}

void Camera::setTarget(const Vector3& newTarget) {
    target = newTarget;

    updateViewMatrix();
}

const Matrix4& Camera::getViewMatrix() const {
    return viewMatrix;
}

const Matrix4& Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

const Vector3& Camera::getUp() const {
    return up;
}

const Vector3& Camera::getTarget() const {
    return target;
}

const Vector3& Camera::getEye() const {
    return eye;
}

const Vector3 Camera::getForward() const {
    return (target - eye).normalized();
}

const Vector3 Camera::getRight() const {
    return getForward().cross(Vector3(0.0f, 1.0f, 0.0f)).normalized();
}

void Camera::rotateOn(float pitch, float yaw) {
    Vector3 forward = getForward().normalized();
    Vector3 right = getRight().normalized();

    Quaternion yawRotation   = Quaternion().FromAxisAngle(Vector3(0, 1, 0), yaw);
    Quaternion pitchRotation = Quaternion().FromAxisAngle(right, pitch);

    Quaternion rotation = pitchRotation * yawRotation;

    forward = rotation.rotate(forward).normalized();
    target = eye + forward;
    updateViewMatrix();
}

} // namespace lw
