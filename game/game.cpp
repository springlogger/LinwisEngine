#include <lw/core/Application.h>
#include <lw/helpers/LoadObj.h>
#include <lw/helpers/AxesHelper.h>
#include <lw/math/Vector3.h>

#include <windows.h>

class Game : public lw::Application {
public:
    Game(lw::DemoConfig cfg) : Application(cfg) {}

    void onInit() override
    {
        lw::Mesh* suzane = scene.addObject(lw::loadObj("C:/Program1/c++/LinwisEngine/assets/suzane.obj"));
        suzane->setPosition(lw::Vector3(0.0f, 0.0f, 0.0f));
        suzane->setRotation(lw::Vector3(0.4f, 0.6f, 0.0f));
        suzane->setScale(lw::Vector3(1.0f, 1.0f, 1.0f));

        lw::AxesHelper* axes = scene.addObject(lw::AxesHelper());
        axes->setScale(lw::Vector3(10.0f, 10.0f, 10.0f));
    }

    void onUpdate(float /*dt*/, const lw::KeyboardInputState& /*kb*/, const lw::MouseInputState& /*mouse*/) override
    {
        // game-specific per-frame logic goes here
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    lw::DemoConfig cfg;
    cfg.devMode = true;

    Game game(cfg);
    game.run(hInstance, nCmdShow);
    return 0;
}
