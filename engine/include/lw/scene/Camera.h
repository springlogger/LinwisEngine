#pragma once

#include <lw/math/Matrix4.h>
#include <lw/math/Vector3.h>

namespace lw {

class Camera {
public:
    Camera();
    Camera(
        const Vector3& eye,
        const Vector3& target,
        const Vector3& up,
        float fovY,
        float aspect,
        float nearPlane,
        float farPlane
    );

    void lookAt(const Vector3& newTarget);
    void setPosition(const Vector3& newPosition);
    void setTarget(const Vector3& newTarget);
    void setAspect(float newAspect);

    void updateViewMatrix();
    void updateProjectionMatrix();

    void rotateOn(float pitch, float yaw);

    const Matrix4& getViewMatrix() const;
    const Matrix4& getProjectionMatrix() const;

    const Vector3& getUp() const;
    const Vector3& getTarget() const;
    const Vector3& getEye() const;

    const Vector3 getForward() const;
    const Vector3 getRight() const;

    float getNearPlane() const { return nearPlane; }
    float getFarPlane() const { return farPlane; }

private:
    Vector3 eye;
    Vector3 target;
    Vector3 up;

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;

    float fovY;
    float aspect;
    float nearPlane;
    float farPlane;
};

} // namespace lw
