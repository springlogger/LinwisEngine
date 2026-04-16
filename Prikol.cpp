// #include "Camera.h"
// #include "Cube.h"
// #include "Math.h"
// #include "Renderer.h"
// #include "Vector3.h"

// #include <chrono>
// #include <windows.h>

// namespace {

// struct DemoConfig {
//     int screenWidth = 120;
//     int screenHeight = 40;
//     float cameraFovDegrees = 60.0f;
//     float nearPlane = 0.1f;
//     float farPlane = 10.0f;
//     float cubeAngularSpeed = lw::math::pi / 6.0f;
// };

// class ConsoleSession {
// public:
//     ConsoleSession() : console(GetStdHandle(STD_OUTPUT_HANDLE)) {
//         SetConsoleOutputCP(CP_UTF8);

//         CONSOLE_CURSOR_INFO cursorInfo{};
//         GetConsoleCursorInfo(console, &cursorInfo);
//         previousCursorVisibility = cursorInfo.bVisible;
//         cursorInfo.bVisible = FALSE;
//         SetConsoleCursorInfo(console, &cursorInfo);
//     }

//     ~ConsoleSession() {
//         CONSOLE_CURSOR_INFO cursorInfo{};
//         GetConsoleCursorInfo(console, &cursorInfo);
//         cursorInfo.bVisible = previousCursorVisibility;
//         SetConsoleCursorInfo(console, &cursorInfo);
//     }

// private:
//     HANDLE console;
//     BOOL previousCursorVisibility = TRUE;
// };

// lw::Camera createCamera(const DemoConfig& config) {
//     const float aspect = static_cast<float>(config.screenWidth) / static_cast<float>(config.screenHeight);

//     return lw::Camera(
//         lw::Vector3(3.0f, 2.0f, 4.0f),
//         lw::Vector3(0.0f, 0.0f, 0.0f),
//         lw::Vector3(0.0f, 1.0f, 0.0f),
//         lw::math::degToRad(config.cameraFovDegrees),
//         aspect,
//         config.nearPlane,
//         config.farPlane
//     );
// }

// } // namespace

// int main() {
//     const ConsoleSession consoleSession;
//     const DemoConfig config;

//     lw::Camera camera = createCamera(config);
//     lw::Cube cube(
//         lw::Vector3(0.0f, 0.0f, 0.0f),
//         lw::Vector3(0.4f, 0.6f, 0.0f),
//         lw::Vector3(1.0f, 1.0f, 1.0f)
//     );
//     lw::Renderer renderer(config.screenWidth, config.screenHeight);

//     using Clock = std::chrono::steady_clock;
//     auto previousFrameTime = Clock::now();
//     float angle = 0.0f;

//     while (true) {
//         const auto currentFrameTime = Clock::now();
//         const std::chrono::duration<float> deltaTime = currentFrameTime - previousFrameTime;
//         previousFrameTime = currentFrameTime;

//         angle += config.cubeAngularSpeed * deltaTime.count();
//         cube.setRotation(lw::Vector3(0.4f, angle, 0.0f));
//         cube.updateMatrix();

//         renderer.render(cube, camera);
//     }
// }
