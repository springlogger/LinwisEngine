#pragma once

#include <chrono>
#include "math.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "RenderTarget.h"
#include "Win32Window.h"

namespace lw
{

using Clock = std::chrono::steady_clock;

struct DemoConfig {
    int screenWidth = 800;
    int screenHeight = 600;
    float cameraFovDegrees = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 10.0f;
    float cubeAngularSpeed = lw::math::pi / 6.0f;
};

class LinwisEngine {
  public:

    Clock::time_point previousFrameTime;
    DemoConfig config;
    lw::Renderer renderer;
    lw::Mesh scene;
    lw::Camera camera;
    float angle;
    float moveSpeed;

    bool isMovingForward;

    LinwisEngine();
    void render();
    void update(const KeyboardInputState& keyboard, const MouseInputState& mouse);
    const Framebuffer& GetFramebuffer() const { return renderer.GetFramebuffer(); };

    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);

    void setIsMovingForward(bool val) { isMovingForward = val; };
};

}