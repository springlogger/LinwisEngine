#pragma once

#include <chrono>
#include "math.h"
#include "Cube.h"
#include "Renderer.h"
#include "Camera.h"

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
    lw::Cube scene;
    lw::Camera camera;
    float angle;

    LinwisEngine();
    void render();
    const Framebuffer& GetFramebuffer() const { return renderer.GetFramebuffer(); };
};