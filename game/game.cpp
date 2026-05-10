#include <lw/core/Application.h>
#include <lw/core/Paths.h>
#include <lw/helpers/LoadObj.h>
#include <lw/helpers/AxesHelper.h>
#include <lw/graphics/Light.h>
#include <lw/math/Vector3.h>
#include <lw/scene/CreateBaseGeometry.h>

#include <windows.h>

class Game : public lw::Application {
public:
    Game(lw::DemoConfig cfg) : Application(cfg) {}

    lw::Mesh* suzane = nullptr;
    float suzaneRotation = 0.0f;

    void onInit() override
    {
        // --- Объект без текстуры ---
        suzane = scene.addObject(lw::loadObj(lw::assetPath("suzane.obj")));
        suzane->setPosition(lw::Vector3(0.0f, 0.0f, 0.0f));
        suzane->setRotation(lw::Vector3(0.4f, 0.6f, 0.0f));
        suzane->setScale(lw::Vector3(1.0f, 1.0f, 1.0f));
        suzane->setTexture(lw::assetPath("test_texture.png"));

        // cube = scene.addObject(
        //     lw::Mesh(
        //         lw::createCubeGeometry(1.5f, 1.5f, 1.5f),
        //         lw::Material()
        //     )
        // );
        // cube->setPosition(lw::Vector3(0.0f, 0.0f, 0.0f));
        // cube->setRotation(lw::Vector3(0.35f, 0.55f, 0.0f));
        // cube->setScale(lw::Vector3(1.0f, 1.0f, 1.0f));

        // --- Объект с текстурой прямо в loadObj ---
        // lw::Mesh* cube = scene.addObject(
        //     lw::loadObj("assets/cube.obj", "assets/cube.png")
        // );

        lw::DirectionalLight keyLight;
        keyLight.direction = lw::Vector3(-0.4f, -0.7f, -0.6f);
        keyLight.ambient = 0.35f;
        keyLight.diffuseStrength = 0.75f;
        keyLight.specularStrength = 0.25f;
        keyLight.shininess = 48.0f;
        scene.setDirectionalLight(keyLight);

        // lw::AxesHelper* axes = scene.addObject(lw::AxesHelper());
        // axes->setScale(lw::Vector3(10.0f, 10.0f, 10.0f));
    }

    void onUpdate(float dt, const lw::KeyboardInputState& /*kb*/, const lw::MouseInputState& /*mouse*/) override
    {
        if (!suzane) {
            return;
        }

        suzaneRotation += dt * 0.2f;
        suzane->setRotation(lw::Vector3(0.35f, suzaneRotation, suzaneRotation * 0.35f));
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    lw::DemoConfig cfg;
    cfg.devMode = false;

    Game game(cfg);
    game.run(hInstance, nCmdShow);
    return 0;
}
