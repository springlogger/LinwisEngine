#pragma once

#include <lw/scene/Scene.h>
#include <lw/graphics/RenderTarget.h>
#include <lw/graphics/RenderableObject.h>
#include <lw/scene/Object3D.h>
#include <lw/scene/Camera.h>
#include <lw/scene/Mesh.h>
#include <lw/scene/LineSegments.h>
#include <vector>
#include <memory>

namespace lw {

class Renderer
{
public:
    Renderer();
    Renderer(int screenWidth, int screenHeight);

    int width() const { return screenWidth; }
    int height() const { return screenHeight; }

    void render(const Scene& scene, const Camera& camera);

    void setWireframe(bool enabled) { wireframe = enabled; }

    const Framebuffer& GetFramebuffer() const { return renderTarget.color; }

private:
    int screenWidth;
    int screenHeight;
    RenderTarget renderTarget;
    bool wireframe = false;

    void clearBuffer();

    void renderMesh(const Mesh& mesh, const Camera& camera);
    void renderLineSegments(const LineSegments& lineSegments, const Camera& camera);
};

}