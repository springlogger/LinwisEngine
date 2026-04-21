#include <lw/platform/win32/Win32Window.h>
#include <lw/core/LinwisEngine.h>

#include <chrono>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    lw::Win32Window window(hInstance);
    if (!window.Create(800, 600, L"My Engine", nCmdShow))
        return -1;

    lw::LinwisEngine engine;

    while (!window.ShouldClose())
    {
        window.PollEvents();

        engine.update(window.GetKeyboardInput(), window.GetMouseInput());
        engine.render();

        window.Present(engine.GetFramebuffer());
    }

    return 0;
}