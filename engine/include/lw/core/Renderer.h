#pragma once

#include <lw/core/RenderTarget.h>
#include <lw/core/Object3D.h>
#include <lw/core/Camera.h>
#include <lw/core/Mesh.h>

#include <vector>
#include <memory>

namespace lw {

struct ScreenVertex {
    float x = 0;
    float y = 0;
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

    void render(const std::vector<std::shared_ptr<Object3D>> scene, const Camera& camera);

    const Framebuffer& GetFramebuffer() const { return renderTarget.color; };

private:
    int screenWidth;
    int screenHeight;
    RenderTarget renderTarget;

    void clearBuffer();
    void drawPoint(int x, int y, uint32_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
    std::vector<ScreenVertex> projectVertices(const Mesh& scene, const Camera& camera) const;
    void drawWireframe(const std::vector<ScreenVertex>& projected, const std::vector<uint32_t>& indices);
    void drawFilledTriangles(const std::vector<ScreenVertex>& projected, const std::vector<uint32_t>& indices, uint32_t color);
};

}