#include "Renderer.h"
#include "Vector4.h"
#include "Object3D.h"
#include "Camera.h"

#include <cmath>
#include <algorithm>

namespace lw {

Renderer::Renderer()
    : screenWidth(800),
      screenHeight(600),
      framebuffer(800, 600)
{
}

Renderer::Renderer(int screenWidth, int screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      framebuffer(screenWidth, screenHeight)
{
}

void Renderer::clearBuffer()
{
    framebuffer.Clear(0x00202020);
}

void Renderer::drawPoint(int x, int y, uint32_t color)
{
    framebuffer.PutPixel(x, y, color);
}

void Renderer::drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    const int dx = std::abs(x1 - x0);
    const int dy = std::abs(y1 - y0);
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;

    int error = dx - dy;

    while (true) {
        drawPoint(x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        const int doubledError = 2 * error;
        if (doubledError > -dy) {
            error -= dy;
            x0 += sx;
        }
        if (doubledError < dx) {
            error += dx;
            y0 += sy;
        }
    }
}

std::vector<ScreenPoint> Renderer::projectVertices(const Object3D& scene, const Camera& camera) const
{
    const auto& vertices = scene.getVertices();
    std::vector<ScreenPoint> projected(vertices.size());

    for (std::size_t i = 0; i < vertices.size(); ++i) {
        const Vector3 vertexWorld = scene.getMatrix().transformPoint(vertices[i]);
        const Vector3 vertexView = camera.getViewMatrix().transformPoint(vertexWorld);
        const Vector4 vertexClip = camera.getProjectionMatrix().transform(Vector4(vertexView.x, vertexView.y, vertexView.z, 1.0f));

        if (std::abs(vertexClip.w) < 1e-6f) {
            continue;
        }

        const float xNdc = vertexClip.x / vertexClip.w;
        const float yNdc = vertexClip.y / vertexClip.w;
        const float zNdc = vertexClip.z / vertexClip.w;

        projected[i] = {
            static_cast<int>((xNdc + 1.0f) * 0.5f * static_cast<float>(screenWidth)),
            static_cast<int>((1.0f - yNdc) * 0.5f * static_cast<float>(screenHeight)),
            zNdc,
            zNdc >= -1.0f && zNdc <= 1.0f
        };
    }

    return projected;
}

void Renderer::drawWireframe(const std::vector<ScreenPoint>& projected, const std::vector<Edge>& edges)
{
    for (const auto& [a, b] : edges) {
        if (a < 0 || b < 0) {
            continue;
        }

        const auto first = static_cast<std::size_t>(a);
        const auto second = static_cast<std::size_t>(b);
        if (first >= projected.size() || second >= projected.size()) {
            continue;
        }

        if (projected[first].visible && projected[second].visible) {
            drawLine(projected[first].x, projected[first].y, projected[second].x, projected[second].y, 0x00FFFFFF);
        }
    }

    for (const auto& point : projected) {
        if (point.visible) {
            drawPoint(point.x, point.y, 0x0000FF00);
        }
    }
}

void Renderer::render(const Object3D& scene, const Camera& camera)
{
    if (!scene.hasGeometry()) {
        return;
    }

    clearBuffer();
    drawWireframe(projectVertices(scene, camera), scene.getEdges());
}

}