#include <lw/graphics/Clip.h>

namespace lw
{

struct ClipPlane
{
    float a, b, c, d;

    // n⋅(P−P0​)=0
    // вообще выводиться через нормаль и точку на плоскости, но так как у нас плоскости заданы в виде x + w >= 0 и так далее, 
    // то можно просто хранить коэффициенты для вычисления f(P) = a * x + b * y + c * z + d * w, 
    // где P - точка в клип-пространстве. И если f(P) >= 0, то точка внутри, иначе снаружи. 
    // Это эквивалентно тому что нормаьль умноженная на (P - P0) >= 0, 
    // где P0 - точка на плоскости, а n - нормаль к плоскости. 
    float evaluate(const Vector4& p) const
    {
        return a * p.x + b * p.y + c * p.z + d * p.w;
    }
};

// Границы заданы тем что -1 <= xNdc <= 1 или -1 <= xNdc <= 1 или (x + w >= 0 ) и так для всех
// вообще x+w = 0 задаёт плоскость впринципе, так как n⋅(P−P0​) = 0 и можно представить 1*x + 0*y + 0*z + 1*w >= 0, где a = 1, b = 0, c = 0, d = 1. И так для всех остальных плоскостей.
// где n = (1, 0, 0) для левой плоскости, n = (-1, 0, 0) для правой плоскости и т.д. 
static const ClipPlane ClipPlanes[] = {
    {  1.0f,  0.0f,  0.0f, 1.0f }, // left:   x + w >= 0
    { -1.0f,  0.0f,  0.0f, 1.0f }, // right: -x + w >= 0

    {  0.0f,  1.0f,  0.0f, 1.0f }, // bottom: y + w >= 0
    {  0.0f, -1.0f,  0.0f, 1.0f }, // top:   -y + w >= 0

    {  0.0f,  0.0f,  1.0f, 1.0f }, // near:   z + w >= 0
    {  0.0f,  0.0f, -1.0f, 1.0f }, // far:   -z + w >= 0
};

// P(t) = A + (B - A) * t,
// где: (B - A) - направление от A к B, где A - начальная точка отрезка, B - конечная. t - параметр, который изменяется от 0 до 1.
// Когда t = 0, P(t) = A, когда t = 1, P(t) = B.

// получаем точку пересечения ребра треугольника с плоскостью отсечения.
static ClipVertex LerpClipVertex(
    const ClipVertex& a,
    const ClipVertex& b,
    float t)
{
    ClipVertex out;

    out.clipPosition = a.clipPosition + (b.clipPosition - a.clipPosition) * t;
    out.color = a.color;
    out.uv.x = a.uv.x + (b.uv.x - a.uv.x) * t;
    out.uv.y = a.uv.y + (b.uv.y - a.uv.y) * t;

    return out;
}
// Задача на самом деле простоя, если плоскость и прямая, нужно найти точку пересечения.
// Плоскость у нас есть через w коэфициент. Но так же плосксть задаётся через общее уровнение скалярного произведения,
// где скалярное произведение равно 0. Иначе мы получаем расстояние от точки до плоскости. 
// И если это расстояние больше 0, то точка внутри, иначе снаружи.
// f(P(t)) = f(A)+t⋅(f(B)−f(A)). ищем f(P(t)) = 0, 
// где P(t) = A + (B - A) * t, A - точка внутри, B - точка снаружи, P(t) - какая-то точка на прямой. 
// Решая это уравнение получаем t = f(A) / (f(A) - f(B)). 
// Подставляем t в P(t) и получаем точку пересечения ребра треугольника с плоскостью отсечения.
static std::vector<ClipVertex> ClipPolygonAgainstPlane(
    const std::vector<ClipVertex>& input,
    const ClipPlane& plane)
{
    std::vector<ClipVertex> output;

    if (input.empty()) {
        return output;
    }

    ClipVertex previous = input.back(); // берём последнюю чтобы сразу обработать ребро от конца к началу
    float previousDistance = plane.evaluate(previous.clipPosition);
    bool previousInside = previousDistance >= 0.0f;

    for (const ClipVertex& current : input)
    {
        const float currentDistance =
            plane.evaluate(current.clipPosition);

        const bool currentInside =
            currentDistance >= 0.0f;

        if (previousInside && currentInside)
        {
            output.push_back(current);
        }
        else if (previousInside && !currentInside)
        {
            const float t =
                previousDistance / (previousDistance - currentDistance);

            output.push_back(
                LerpClipVertex(previous, current, t)
            );
        }
        else if (!previousInside && currentInside)
        {
            const float t =
                previousDistance / (previousDistance - currentDistance);

            output.push_back(
                LerpClipVertex(previous, current, t)
            );

            output.push_back(current);
        }

        previous = current;
        previousDistance = currentDistance;
        previousInside = currentInside;
    }

    return output;
}

static std::vector<ClipVertex> ClipTriangleToPolygon(
    const ClipVertex& a,
    const ClipVertex& b,
    const ClipVertex& c)
{
    std::vector<ClipVertex> polygon = { a, b, c };

    for (const ClipPlane& plane : ClipPlanes)
    {
        polygon = ClipPolygonAgainstPlane(polygon, plane);

        if (polygon.empty()) {
            break;
        }
    }

    return polygon;
}

//fan triangulation
//Преобразуем многогранник в треугольники
static std::vector<ClipTriangle> TriangulateFan(
  const std::vector<ClipVertex>& polygon)
{
    std::vector<ClipTriangle> triangles;

    if (polygon.size() < 3) {
        return triangles;
    }

    triangles.reserve(polygon.size() - 2);

    for (std::size_t i = 1; i + 1 < polygon.size(); ++i)
    {
        triangles.push_back({
            polygon[0],
            polygon[i],
            polygon[i + 1]
        });
    }

    return triangles;
}

std::vector<ClipTriangle> ClipTriangles(
    const std::vector<ClipTriangle>& triangles)
{
    std::vector<ClipTriangle> output;

    for (const ClipTriangle& triangle : triangles)
    {
        std::vector<ClipVertex> polygon =
            ClipTriangleToPolygon(
                triangle.v0,
                triangle.v1,
                triangle.v2
            );

        if (polygon.size() < 3) {
            continue;
        }

        std::vector<ClipTriangle> triangulated =
            TriangulateFan(polygon);

        output.insert(
            output.end(),
            triangulated.begin(),
            triangulated.end()
        );
    }

    return output;
}

bool ClipLine(ClipVertex& a, ClipVertex& b)
{
    for (const ClipPlane& plane : ClipPlanes)
    {
        const float da = plane.evaluate(a.clipPosition);
        const float db = plane.evaluate(b.clipPosition);

        const bool aIn = da >= 0.0f;
        const bool bIn = db >= 0.0f;

        if (!aIn && !bIn) return false;

        if (!aIn) {
            const float t = da / (da - db);
            a = LerpClipVertex(a, b, t);
        } else if (!bIn) {
            const float t = da / (da - db);
            b = LerpClipVertex(a, b, t);
        }
    }
    return true;
}

}