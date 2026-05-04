#include <lw/graphics/Renderer.h>
#include <lw/graphics/Clip.h>
#include <lw/graphics/Rasterizer.h>

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

static ScreenVertex ToScreenVertex(
    const ClipVertex& v,
    int screenWidth,
    int screenHeight)
{
    const Vector4& p = v.clipPosition;

    const float invW = 1.0f / p.w;

    const float xNdc = p.x * invW;
    const float yNdc = p.y * invW;
    const float zNdc = p.z * invW;

    ScreenVertex out;
    out.x = (xNdc + 1.0f) * 0.5f * static_cast<float>(screenWidth);
    out.y = (1.0f - yNdc) * 0.5f * static_cast<float>(screenHeight);
    out.z = zNdc;

    return out;
}

static std::vector<ScreenTriangle> ProjectTrianglesToScreen(
    const std::vector<ClipTriangle>& triangles,
    int screenWidth,
    int screenHeight)
{
    std::vector<ScreenTriangle> output;
    output.reserve(triangles.size());

    for (const ClipTriangle& triangle : triangles)
    {
        output.push_back({
            ToScreenVertex(triangle.v0, screenWidth, screenHeight),
            ToScreenVertex(triangle.v1, screenWidth, screenHeight),
            ToScreenVertex(triangle.v2, screenWidth, screenHeight)
        });
    }

    return output;
}

static std::vector<ClipTriangle> AssembleTriangles(
    const std::vector<ClipVertex>& vertices,
    const std::vector<uint32_t>& indices)
{
    std::vector<ClipTriangle> triangles;

    if (indices.size() % 3 != 0) {
        return triangles;
    }

    triangles.reserve(indices.size() / 3);

    for (std::size_t i = 0; i < indices.size(); i += 3)
    {
        const uint32_t i0 = indices[i];
        const uint32_t i1 = indices[i + 1];
        const uint32_t i2 = indices[i + 2];

        if (i0 >= vertices.size() ||
            i1 >= vertices.size() ||
            i2 >= vertices.size())
        {
            continue;
        }

        triangles.push_back({
            vertices[i0],
            vertices[i1],
            vertices[i2]
        });
    }

    return triangles;
}

template<typename TVertex>
static ClipVertex ProcessVertex(
    const TVertex& vertex,
    const Matrix4& model,
    const Matrix4& view,
    const Matrix4& projection)
{
    const Vector3 world =
        model.transformPoint(vertex.position);

    const Vector3 viewPos =
        view.transformPoint(world);

    const Vector4 clip =
        projection.transform(Vector4(viewPos.x, viewPos.y, viewPos.z, 1.0f));

    ClipVertex out;
    out.clipPosition = clip;
    out.color = vertex.color;

    return out;
}

template<typename TVertex>
static std::vector<ClipVertex> ProcessVertices(
    const std::vector<TVertex>& vertices,
    const Matrix4& model,
    const Camera& camera)
{
    std::vector<ClipVertex> output;
    output.reserve(vertices.size());

    const Matrix4 view = camera.getViewMatrix();
    const Matrix4 projection = camera.getProjectionMatrix();

    for (const TVertex& vertex : vertices)
    {
        output.push_back(ProcessVertex(vertex, model, view, projection));
    }

    return output;
}

void Renderer::renderMesh(const Mesh& mesh, const Camera& camera)
{
    const MeshGeometry& geometry = mesh.getGeometry();

    // 1. Vertex processing
    const std::vector<ClipVertex> clipVertices =
        ProcessVertices(
            geometry.vertices,
            mesh.getMatrix(),
            camera
        );

    // 2. Primitive assembly
    const std::vector<ClipTriangle> triangles =
        AssembleTriangles(
            clipVertices,
            geometry.indices
        );

    // 3. Clipping + triangulation
    const std::vector<ClipTriangle> clippedTriangles =
        ClipTriangles(triangles);

    // 4. Perspective divide + viewport
    const std::vector<ScreenTriangle> screenTriangles =
        ProjectTrianglesToScreen(
            clippedTriangles,
            screenWidth,
            screenHeight
        );

    // 5. Rasterization
    RasterizeTriangles(renderTarget, screenTriangles, 0x00000033);

    // 6. Wireframe pass
    if (wireframe) {
        RasterizeWireframe(renderTarget, screenTriangles, 0x00FFFFFF);
    }
}

static std::vector<ClipSegment> AssembleSegments(
    const std::vector<ClipVertex>& vertices,
    const std::vector<uint32_t>& indices)
{
    std::vector<ClipSegment> segments;

    if (indices.size() % 2 != 0) {
        return segments;
    }

    segments.reserve(indices.size() / 2);

    for (std::size_t i = 0; i < indices.size(); i += 2)
    {
        const uint32_t i0 = indices[i];
        const uint32_t i1 = indices[i + 1];

        if (i0 >= vertices.size() || i1 >= vertices.size()) {
            continue;
        }

        segments.push_back({ vertices[i0], vertices[i1] });
    }

    return segments;
}

static std::vector<ClipSegment> ClipSegments(
    std::vector<ClipSegment> segments)
{
    std::vector<ClipSegment> output;

    for (ClipSegment& seg : segments)
    {
        if (ClipLine(seg.a, seg.b)) {
            output.push_back(seg);
        }
    }

    return output;
}

static std::vector<ScreenSegment> ProjectSegmentsToScreen(
    const std::vector<ClipSegment>& segments,
    int screenWidth,
    int screenHeight)
{
    std::vector<ScreenSegment> output;
    output.reserve(segments.size());

    for (const ClipSegment& seg : segments)
    {
        output.push_back({
            ToScreenVertex(seg.a, screenWidth, screenHeight),
            ToScreenVertex(seg.b, screenWidth, screenHeight),
            seg.a.color
        });
    }

    return output;
}

void Renderer::renderLineSegments(const LineSegments& lineSegments, const Camera& camera)
{
    const LineGeometry& geometry = lineSegments.getGeometry();

    // 1. Vertex processing
    const std::vector<ClipVertex> clipVertices =
        ProcessVertices(
            geometry.vertices,
            lineSegments.getMatrix(),
            camera
        );

    // 2. Primitive assembly
    const std::vector<ClipSegment> segments =
        AssembleSegments(
            clipVertices,
            geometry.indices
        );

    // 3. Clipping
    const std::vector<ClipSegment> clippedSegments =
        ClipSegments(segments);

    // 4. Perspective divide + viewport
    const std::vector<ScreenSegment> screenSegments =
        ProjectSegmentsToScreen(
            clippedSegments,
            screenWidth,
            screenHeight
        );

    // 5. Rasterization
    RasterizeLines(renderTarget, screenSegments);
}


void Renderer::render(const Scene& scene, const Camera& camera)
{
    clearBuffer();
    for (const auto& object : scene.getObjects()) {

        const RenderableObject* renderableObject = dynamic_cast<const RenderableObject*>(object.get());
        
        if (!renderableObject) continue;

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