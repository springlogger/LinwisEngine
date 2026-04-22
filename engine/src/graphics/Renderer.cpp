#include <lw/graphics/Renderer.h>

#include <cmath>
#include <algorithm>

namespace lw {

Renderer::Renderer()
    : screenWidth(800),
      screenHeight(600),
      renderTarget(800, 600)
{
}

Renderer::Renderer(int screenWidth, int screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      renderTarget(screenWidth, screenHeight)
{
}

void Renderer::clearBuffer()
{
    renderTarget.color.Clear(0x00202020);
    renderTarget.depth.Clear(1.0f);
}

void Renderer::drawPoint(int x, int y, uint32_t color)
{
    renderTarget.color.PutPixel(x, y, color);
}

static float EdgeFunction(
    float ax, float ay,
    float bx, float by,
    float px, float py)
{
    return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
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

void Renderer::drawFilledTriangles(const std::vector<ScreenVertex>& projected, const std::vector<uint32_t>& indices, uint32_t color) {

    for (std::size_t i = 0; i < (indices.size() - 2); i += 3) {

        const ScreenVertex& v0 = projected[indices[i]];
        const ScreenVertex& v1 = projected[indices[i + 1]];
        const ScreenVertex& v2 = projected[indices[i + 2]];

        if (!v0.visible || !v1.visible || !v2.visible) {
            continue;
        }

        const float area = EdgeFunction(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
        if (std::abs(area) < 1e-6f) {
            continue;
        }

        if (area < 0.0f) {
            continue;
}
        const int minX = std::max(
            0,
            static_cast<int>(std::floor(std::min({v0.x, v1.x, v2.x})))
        );

        const int maxX = std::min(
            renderTarget.color.width - 1,
            static_cast<int>(std::ceil(std::max({v0.x, v1.x, v2.x})))
        );

        const int minY = std::max(
            0,
            static_cast<int>(std::floor(std::min({v0.y, v1.y, v2.y})))
        );

        const int maxY = std::min(
            renderTarget.color.height - 1,
            static_cast<int>(std::ceil(std::max({v0.y, v1.y, v2.y})))
        );

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                const float px = static_cast<float>(x) + 0.5f;
                const float py = static_cast<float>(y) + 0.5f;

                // напротив v0, поэтому если бы p = v0, то мы бы получили α = 1. От него считаем долю по v0
                const float w0 = EdgeFunction(v1.x, v1.y, v2.x, v2.y, px, py);
                const float w1 = EdgeFunction(v2.x, v2.y, v0.x, v0.y, px, py);
                const float w2 = EdgeFunction(v0.x, v0.y, v1.x, v1.y, px, py);

                const bool inside =
                    (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f);

                if (!inside) {
                    continue;
                }

                // P=αv0​+βv1​+γv2​
                const float alpha = w0 / area;
                const float beta  = w1 / area;
                const float gamma = w2 / area;

                // zP​=αz0​+βz1​+γz2​
                const float depth =
                    alpha * v0.z +
                    beta  * v1.z +
                    gamma * v2.z;

                if (depth < renderTarget.depth.At(x, y))
                {
                    renderTarget.depth.At(x, y) = depth;
                    renderTarget.color.PutPixel(x, y, color);
                }
            }
        }

    }
}

void Renderer::drawLineSegment(
    const ScreenVertex& a,
    const ScreenVertex& b,
    uint32_t color)
{
    if (!a.visible || !b.visible) {
        return;
    }

    drawLine(
        static_cast<int>(a.x),
        static_cast<int>(a.y),
        static_cast<int>(b.x),
        static_cast<int>(b.y),
        color
    );
}

void Renderer::drawWireframe(
    const std::vector<ScreenVertex>& projected,
    const std::vector<uint32_t>& indices)
{
    if (indices.size() % 3 != 0) {
        return;
    }

    for (std::size_t i = 0; i < indices.size(); i += 3)
    {
        const uint32_t i0 = indices[i];
        const uint32_t i1 = indices[i + 1];
        const uint32_t i2 = indices[i + 2];

        if (i0 >= projected.size() || i1 >= projected.size() || i2 >= projected.size()) {
            continue;
        }

        drawLineSegment(projected[i0], projected[i1], 0x00FFFFFF);
        drawLineSegment(projected[i1], projected[i2], 0x00FFFFFF);
        drawLineSegment(projected[i2], projected[i0], 0x00FFFFFF);
    }
}

void Renderer::drawLineIndices(
    const std::vector<ScreenVertex>& projected,
    const std::vector<LineVertex>& vertices,
    const std::vector<uint32_t>& indices)
{
    if (indices.size() % 2 != 0) {
        return;
    }

    for (std::size_t i = 0; i < indices.size(); i += 2)
    {
        const uint32_t i0 = indices[i];
        const uint32_t i1 = indices[i + 1];

        if (i0 >= projected.size() || i1 >= projected.size()) {
            continue;
        }

        drawLineSegment(projected[i0], projected[i1], vertices[i0].color);
    }
}

template<typename TVertex>
static std::vector<ScreenVertex> projectVertexArray(const std::vector<TVertex>& vertices, const Matrix4& objectMatrix, const Camera& camera, int screenWidth, int screenHeight) {
    std::vector<ScreenVertex> projected(vertices.size());

    for (std::size_t i = 0; i < vertices.size(); ++i) {
        const Vector3 vertexWorld = objectMatrix.transformPoint(vertices[i].position);
        const Vector3 vertexView = camera.getViewMatrix().transformPoint(vertexWorld);
        const Vector4 vertexClip = camera.getProjectionMatrix().transform(
            Vector4(vertexView.x, vertexView.y, vertexView.z, 1.0f)
        );

        if (std::abs(vertexClip.w) < 1e-6f) {
            continue;
        }

        const float xNdc = vertexClip.x / vertexClip.w;
        const float yNdc = vertexClip.y / vertexClip.w;
        const float zNdc = vertexClip.z / vertexClip.w;

        projected[i] = {
            ((xNdc + 1.0f) * 0.5f * static_cast<float>(screenWidth)),
            ((1.0f - yNdc) * 0.5f * static_cast<float>(screenHeight)),
            zNdc,
            zNdc >= -1.0f && zNdc <= 1.0f
        };
    }

    return projected;
}

std::vector<ScreenVertex> Renderer::projectVertices(const Mesh& object, const Camera& camera) const
{
    return projectVertexArray(object.getGeometry().vertices, object.getMatrix(), camera, screenWidth, screenHeight);
}

std::vector<ScreenVertex> Renderer::projectVertices(const LineSegments& object, const Camera& camera) const
{
    return projectVertexArray(object.getGeometry().vertices, object.getMatrix(), camera, screenWidth, screenHeight);
}

void Renderer::renderMesh(const Mesh& mesh, const Camera& camera)
{
    const MeshGeometry& geometry = mesh.getGeometry();

    const auto projected = projectVertices(mesh, camera);

    drawFilledTriangles(projected, geometry.indices, 0x00000033);
    drawWireframe(projected, geometry.indices);
}

void Renderer::renderLineSegments(const LineSegments& lineSegments, const Camera& camera)
{
    const LineGeometry& geometry = lineSegments.getGeometry();

    const auto projected = projectVertices(lineSegments, camera);

    drawLineIndices(projected, geometry.vertices, geometry.indices);
}

void Renderer::render(const std::vector<std::shared_ptr<Object3D>> scene, const Camera& camera)
{
    clearBuffer();
    for (const auto& object : scene) {

        const RenderableObject* renderableObject = dynamic_cast<const RenderableObject*>(object.get());

        switch (renderableObject->getPrimitiveType())
        {
        case PrimitiveType::Triangles: {
            const auto* mesh = static_cast<const Mesh*>(renderableObject);
            renderMesh(*mesh, camera);

            break;
        }

        case PrimitiveType::Lines: {
            const auto* lines = static_cast<const LineSegments*>(renderableObject);
            renderLineSegments(*lines, camera);
                
            break;
        }
        
        default:
            break;
        }

    }
}

}