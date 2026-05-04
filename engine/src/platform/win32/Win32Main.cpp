#include <lw/platform/win32/Win32Window.h>
#include <lw/core/LinwisEngine.h>
#include <lw/scene/Mesh.h>
#include <lw/helpers/LoadObj.h>
#include <lw/helpers/AxesHelper.h>
#include <lw/math/Vector3.h>

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    lw::DemoConfig config;
    config.devMode = true;

    lw::Win32Window window(hInstance);
    if (!window.Create(config.screenWidth, config.screenHeight, L"LinwisEngine", nCmdShow))
        return -1;

    lw::LinwisEngine engine(config);

    lw::Mesh* suzane = engine.scene.addObject(lw::loadObj("C:/Program1/c++/LinwisEngine/assets/suzane.obj"));

    suzane->setPosition(lw::Vector3(0.0f, 0.0f, 0.0f));
    suzane->setRotation(lw::Vector3(0.4f, 0.6f, 0.0f));
    suzane->setScale(lw::Vector3(1.0f, 1.0f, 1.0f));

    engine.scene.addObject(lw::AxesHelper());

    while (!window.ShouldClose())
    {
        window.PollEvents();

        engine.update(window.GetKeyboardInput(), window.GetMouseInput());
        engine.render();

        window.Present(engine.GetFramebuffer());
    }

    return 0;
}
