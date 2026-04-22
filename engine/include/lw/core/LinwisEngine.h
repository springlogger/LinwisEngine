#pragma once

#include <lw/math/Math.h>
#include <lw/graphics/Renderer.h>
#include <lw/scene/Camera.h>
#include <lw/scene/Mesh.h>
#include <lw/graphics/RenderTarget.h>
#include <lw/platform/win32/Win32Window.h>

#include <chrono>
#include <vector>
#include <memory>

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
    Renderer renderer;
    std::vector<std::shared_ptr<Object3D>> scene;
    Camera camera;
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