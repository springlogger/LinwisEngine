#include "Win32Window.h"
#include "Renderer.h"
#include "LinwisEngine.h"

#include <chrono>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    Win32Window window(hInstance);
    if (!window.Create(800, 600, L"My Engine", nCmdShow))
        return -1;

    lw::LinwisEngine engine;

    while (!window.ShouldClose())
    {
        window.PollEvents();

        engine.render();

        window.Present(engine.GetFramebuffer());
    }

    return 0;
}