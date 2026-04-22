#include <lw/core/LinwisEngine.h>
#include <lw/graphics/Renderer.h>
#include <lw/scene/Camera.h>
#include <lw/scene/Mesh.h>
#include <lw/math/Math.h>
#include <lw/platform/win32/Win32Window.h>
#include <lw/scene/MeshGeometry.h>
#include <lw/scene/Material.h>
#include <lw/scene/CreateBaseGeometry.h>
#include <lw/helpers/AxesHelper.h>

#include <chrono>
#include <cmath>

using Clock = std::chrono::steady_clock;

namespace lw {

Camera createCamera(const DemoConfig& config) {
    const float aspect = static_cast<float>(config.screenWidth) / static_cast<float>(config.screenHeight);

    return Camera(
        Vector3(3.0f, 2.0f, 4.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        math::degToRad(config.cameraFovDegrees),
        aspect,
        config.nearPlane,
        config.farPlane
    );
}

LinwisEngine::LinwisEngine() {
    camera = createCamera(config);

    MeshGeometry cubeGeometry = createCubeGeometry(2, 2, 2);
    Material cubeMaterial = Material();

    auto cube = std::make_unique<Mesh>(cubeGeometry, cubeMaterial);
    cube->setPosition(Vector3(0.0f, 0.0f, 0.0f));
    cube->setRotation(Vector3(0.4f, 0.6f, 0.0f));
    cube->setScale(Vector3(1.0f, 1.0f, 1.0f));

    auto axesHelper = std::make_unique<AxesHelper>();
    // axesHelper->setScale(Vector3(10, 10, 10));

    scene.push_back(std::move(cube));
    scene.push_back(std::move(axesHelper));

    renderer = Renderer(config.screenWidth, config.screenHeight);

    previousFrameTime = Clock::now();
    angle = 0.0f;
    moveSpeed = 3.0f;
}

//TODO: добавить обработку функций на верхнем уровне. Камера и сцена должны создаваться на верхем уровне
void LinwisEngine::update(const KeyboardInputState& keyboard, const MouseInputState& mouse) {
    const auto currentFrameTime = Clock::now();
    const std::chrono::duration<float> deltaTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    angle += config.cubeAngularSpeed * deltaTime.count();
    scene[0]->setRotation(lw::Vector3(0.4f, angle, 0.0f));
    scene[0]->updateMatrix();

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