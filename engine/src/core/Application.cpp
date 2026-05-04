#include <lw/core/Application.h>

#include <chrono>

namespace lw {

Application::Application(const DemoConfig& cfg)
    : engine(cfg),
      scene(engine.scene),
      config(cfg)
{}

void Application::run(HINSTANCE hInstance, int nCmdShow)
{
    Win32Window window(hInstance);
    if (!window.Create(config.screenWidth, config.screenHeight, L"LinwisEngine", nCmdShow))
        return;

    onInit();

    auto previousTime = std::chrono::steady_clock::now();

    while (!window.ShouldClose())
    {
        window.PollEvents();

        const auto now = std::chrono::steady_clock::now();
        const float dt = std::chrono::duration<float>(now - previousTime).count();
        previousTime = now;

        const KeyboardInputState& kb    = window.GetKeyboardInput();
        const MouseInputState&    mouse = window.GetMouseInput();

        engine.update(kb, mouse);
        onUpdate(dt, kb, mouse);
        engine.render();

        window.Present(engine.GetFramebuffer());
    }
}

} // namespace lw
