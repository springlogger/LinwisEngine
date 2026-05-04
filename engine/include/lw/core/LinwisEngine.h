#pragma once

#include <lw/scene/Scene.h>
#include <lw/math/Math.h>
#include <lw/graphics/Renderer.h>
#include <lw/scene/Camera.h>
#include <lw/scene/Mesh.h>
#include <lw/graphics/RenderTarget.h>
#include <lw/platform/win32/Win32Window.h>

#include <chrono>
#include <vector>

namespace lw
{

struct DemoConfig {
    int screenWidth = 800;
    int screenHeight = 600;
    float cameraFovDegrees = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 10.0f;
    float cubeAngularSpeed = lw::math::pi / 6.0f;
    bool devMode = false;
};

class LinwisEngine {
public:
    Scene scene;
    explicit LinwisEngine(const DemoConfig& config = DemoConfig{});

    void update(const KeyboardInputState& keyboard, const MouseInputState& mouse);
    void render();

    [[nodiscard]] const Framebuffer& GetFramebuffer() const { return renderer.GetFramebuffer(); }

private:
    DemoConfig config;
    Renderer renderer;
    Camera camera;

    std::chrono::steady_clock::time_point previousFrameTime;
    float angle = 0.0f;
    float moveSpeed = 3.0f;

    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);
};

} // namespace lw
