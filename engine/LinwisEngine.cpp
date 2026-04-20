#include "LinwisEngine.h"
#include "Camera.h"
#include "Renderer.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "CreateBaseGeometry.h"

#include <chrono>
#include <cmath>

using Clock = std::chrono::steady_clock;

namespace lw {

Camera createCamera(const DemoConfig& config) {
    const float aspect = static_cast<float>(config.screenWidth) / static_cast<float>(config.screenHeight);

    return lw::Camera(
        lw::Vector3(3.0f, 2.0f, 4.0f),
        lw::Vector3(0.0f, 0.0f, 0.0f),
        lw::Vector3(0.0f, 1.0f, 0.0f),
        lw::math::degToRad(config.cameraFovDegrees),
        aspect,
        config.nearPlane,
        config.farPlane
    );
}

LinwisEngine::LinwisEngine() {
    camera = createCamera(config);

    lw::Geometry cubeGeometry = lw::createCubeGeometry(2, 2, 2);
    lw::Material cubeMaterial = lw::Material();

    scene = lw::Mesh(cubeGeometry, cubeMaterial);
    scene.setPosition(lw::Vector3(0.0f, 0.0f, 0.0f));
    scene.setRotation(lw::Vector3(0.4f, 0.6f, 0.0f));
    scene.setScale(lw::Vector3(1.0f, 1.0f, 1.0f));

    renderer = lw::Renderer(config.screenWidth, config.screenHeight);

    previousFrameTime = Clock::now();
    angle = 0.0f;
    moveSpeed = 3.0f;
}

void LinwisEngine::update(const KeyboardInputState& keyboard, const MouseInputState& mouse) {
    const auto currentFrameTime = Clock::now();
    const std::chrono::duration<float> deltaTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    angle += config.cubeAngularSpeed * deltaTime.count();
    scene.setRotation(lw::Vector3(0.4f, angle, 0.0f));
    scene.updateMatrix();

    if (mouse.isMouseLookActive && (mouse.dx != 0 || mouse.dy != 0)) {
        const float sens = 0.001f;
        const float yaw = -static_cast<float>(mouse.dx) * sens;
        const float pitch = -static_cast<float>(mouse.dy) * sens;

        camera.rotateOn(pitch, yaw);
    }

    if (keyboard.w) {
        moveForward(deltaTime.count());
    }

    if (keyboard.s) {
        moveBackward(deltaTime.count());
    }

    if (keyboard.a) {
        moveLeft(deltaTime.count());
    }

    if (keyboard.d) {
        moveRight(deltaTime.count());
    }
}

void LinwisEngine::render() {

    renderer.render(scene, camera);

}

void LinwisEngine::moveForward(float delta)
{
    const Vector3 forward = camera.getForward();
    const Vector3 offset = forward * (moveSpeed * delta);

    camera.setPosition(camera.getEye() + offset);
    camera.setTarget(camera.getTarget() + offset);
}

void LinwisEngine::moveBackward(float delta)
{
    const Vector3 forward = camera.getForward();
    const Vector3 offset = forward * (moveSpeed * delta);

    camera.setPosition(camera.getEye() - offset);
    camera.setTarget(camera.getTarget() - offset);
}

void LinwisEngine::moveRight(float delta)
{
    const Vector3 right = camera.getRight();
    const Vector3 offset = right * (moveSpeed * delta);

    camera.setPosition(camera.getEye() + offset);
    camera.setTarget(camera.getTarget() + offset);
}

void LinwisEngine::moveLeft(float delta)
{
    const Vector3 right = camera.getRight();
    const Vector3 offset = right * (moveSpeed * delta);

    camera.setPosition(camera.getEye() - offset);
    camera.setTarget(camera.getTarget() - offset);
}

}