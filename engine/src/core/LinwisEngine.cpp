#include <lw/core/LinwisEngine.h>
#include <lw/scene/MeshGeometry.h>
#include <lw/scene/Material.h>
#include <lw/scene/CreateBaseGeometry.h>
#include <lw/helpers/AxesHelper.h>

#include <chrono>

namespace lw {

static Camera createCamera(const DemoConfig& config)
{
    const float aspect =
        static_cast<float>(config.screenWidth) /
        static_cast<float>(config.screenHeight);

    return Camera(
        Vector3(3.0f, 2.0f, 4.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        math::degToRad(config.cameraFovDegrees),
        aspect,
        config.nearPlane,
        config.farPlane
    );
}

LinwisEngine::LinwisEngine(const DemoConfig& cfg)
    : config(cfg),
      renderer(config.screenWidth, config.screenHeight),
      camera(createCamera(config)),
      previousFrameTime(std::chrono::steady_clock::now())
{
    renderer.setWireframe(config.devMode);

    MeshGeometry cubeGeometry = createCubeGeometry(2.0f, 2.0f, 2.0f);

    auto cube = std::make_unique<Mesh>(cubeGeometry, Material());
    cube->setPosition(Vector3(0.0f, 0.0f, 0.0f));
    cube->setRotation(Vector3(0.4f, 0.6f, 0.0f));
    cube->setScale(Vector3(1.0f, 1.0f, 1.0f));

    scene.push_back(std::move(cube));
    scene.push_back(std::make_unique<AxesHelper>());
}

void LinwisEngine::update(const KeyboardInputState& keyboard, const MouseInputState& mouse)
{
    const auto now = std::chrono::steady_clock::now();
    const float dt = std::chrono::duration<float>(now - previousFrameTime).count();
    previousFrameTime = now;

    angle += config.cubeAngularSpeed * dt;
    scene[0]->setRotation(Vector3(0.4f, angle, 0.0f));
    scene[0]->updateMatrix();

    if (mouse.isMouseLookActive && (mouse.dx != 0 || mouse.dy != 0)) {
        constexpr float sens = 0.001f;
        camera.rotateOn(
            -static_cast<float>(mouse.dy) * sens,
            -static_cast<float>(mouse.dx) * sens
        );
    }

    if (keyboard.w) moveForward(dt);
    if (keyboard.s) moveBackward(dt);
    if (keyboard.a) moveLeft(dt);
    if (keyboard.d) moveRight(dt);
}

void LinwisEngine::render()
{
    renderer.render(scene, camera);
}

void LinwisEngine::moveForward(float dt)
{
    const Vector3 offset = camera.getForward() * (moveSpeed * dt);
    camera.setPosition(camera.getEye() + offset);
    camera.setTarget(camera.getTarget() + offset);
}

void LinwisEngine::moveBackward(float dt)
{
    const Vector3 offset = camera.getForward() * (moveSpeed * dt);
    camera.setPosition(camera.getEye() - offset);
    camera.setTarget(camera.getTarget() - offset);
}

void LinwisEngine::moveRight(float dt)
{
    const Vector3 offset = camera.getRight() * (moveSpeed * dt);
    camera.setPosition(camera.getEye() + offset);
    camera.setTarget(camera.getTarget() + offset);
}

void LinwisEngine::moveLeft(float dt)
{
    const Vector3 offset = camera.getRight() * (moveSpeed * dt);
    camera.setPosition(camera.getEye() - offset);
    camera.setTarget(camera.getTarget() - offset);
}

} // namespace lw
