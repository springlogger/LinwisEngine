#pragma once

#include <lw/core/LinwisEngine.h>
#include <lw/scene/Scene.h>
#include <lw/platform/win32/Win32Window.h>


namespace lw {

class Application {
private:
    LinwisEngine engine;

public:
    Scene& scene;

    explicit Application(const DemoConfig& config = DemoConfig{});
    virtual ~Application() = default;

    virtual void onInit() {}
    virtual void onUpdate(float /*dt*/, const KeyboardInputState& /*keyboard*/, const MouseInputState& /*mouse*/) {}

    void run(HINSTANCE hInstance, int nCmdShow);

protected:
    DemoConfig config;
};

} // namespace lw
