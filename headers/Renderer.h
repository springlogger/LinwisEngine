#pragma once
#include "Framebuffer.h"
#include "Object3D.h"
#include "Camera.h"
#include <vector>

namespace lw {

struct ScreenPoint {
    int x = 0;
    int y = 0;
    float z = 0.0f;
    bool visible = false;
};

class Renderer
{
public:
    Renderer();
    Renderer(int screenWidth, int screenHeight);

    int width() const { return screenWidth; };
    int height() const { return screenHeight; } ;

    void render(const Object3D& scene, const Camera& camera);

    const Framebuffer& GetFramebuffer() const { return framebuffer; };

private:
    int screenWidth;
    int screenHeight;
    Framebuffer framebuffer;

    void clearBuffer();
    void drawPoint(int x, int y, uint32_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
    std::vector<ScreenPoint> projectVertices(const Object3D& scene, const Camera& camera) const;
    void drawWireframe(const std::vector<ScreenPoint>& projected, const std::vector<Edge>& edges);
};

}