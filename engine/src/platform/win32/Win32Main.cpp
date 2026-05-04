#include <lw/platform/win32/Win32Window.h>
#include <lw/core/LinwisEngine.h>

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    lw::DemoConfig config;
    config.devMode = true;

    lw::Win32Window window(hInstance);
    if (!window.Create(config.screenWidth, config.screenHeight, L"LinwisEngine", nCmdShow))
        return -1;

    lw::LinwisEngine engine(config);

    while (!window.ShouldClose())
    {
        window.PollEvents();

        engine.update(window.GetKeyboardInput(), window.GetMouseInput());
        engine.render();

        window.Present(engine.GetFramebuffer());
    }

    return 0;
}
