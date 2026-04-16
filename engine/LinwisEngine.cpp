#include "LinwisEngine.h"
#include "Camera.h"
#include "Renderer.h"
#include "Cube.h"

#include <chrono>
#include <cmath>

using Clock = std::chrono::steady_clock;

namespace {

lw::Camera createCamera(const DemoConfig& config) {
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

}

LinwisEngine::LinwisEngine() {
    camera = createCamera(config);
    scene = lw::Cube(
        lw::Vector3(0.0f, 0.0f, 0.0f),
        lw::Vector3(0.4f, 0.6f, 0.0f),
        lw::Vector3(1.0f, 1.0f, 1.0f)
    );
    renderer = lw::Renderer(config.screenWidth, config.screenHeight);

    previousFrameTime = Clock::now();
    angle = 0.0f;
}

void LinwisEngine::render() {

    const auto currentFrameTime = Clock::now();
    const std::chrono::duration<float> deltaTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    angle += config.cubeAngularSpeed * deltaTime.count();
    scene.setRotation(lw::Vector3(0.4f, angle, 0.0f));
    scene.updateMatrix();

    renderer.render(scene, camera);

}