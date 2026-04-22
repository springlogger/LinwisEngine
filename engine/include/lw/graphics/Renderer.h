#pragma once

#include <lw/graphics/RenderTarget.h>
#include <lw/graphics/RenderableObject.h>
#include <lw/scene/Object3D.h>
#include <lw/scene/Camera.h>
#include <lw/scene/Mesh.h>
#include <lw/scene/LineSegments.h>
#include <lw/math/Vector4.h>
#include <lw/helpers/AxesHelper.h>

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

    void drawLineSegment(const ScreenVertex& a, const ScreenVertex& b, uint32_t color);
    void drawLineIndices(const std::vector<ScreenVertex>& projected, const std::vector<LineVertex>& vertices, const std::vector<uint32_t>& indices);
    void drawWireframe(const std::vector<ScreenVertex>& projected, const std::vector<uint32_t>& indices);
    
    void drawFilledTriangles(const std::vector<ScreenVertex>& projected, const std::vector<uint32_t>& indices, uint32_t color);

    void renderMesh(const Mesh& mesh, const Camera& camera);
    void renderLineSegments(const LineSegments& lineSegments, const Camera& camera);

    std::vector<ScreenVertex> projectVertices(const Mesh& object, const Camera& camera) const;
    std::vector<ScreenVertex> projectVertices(const LineSegments& object, const Camera& camera) const;
};

}