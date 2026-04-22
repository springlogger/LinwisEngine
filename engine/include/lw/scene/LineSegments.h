#pragma once

#include <lw/scene/Object3D.h>
#include <lw/scene/LineGeometry.h>
#include <lw/graphics/RenderableObject.h>

#include <cstdint>

namespace lw
{
class LineSegments : public RenderableObject
{
public:
    LineSegments(LineGeometry geometry) : geometry(geometry){};

    const LineGeometry& getGeometry() const { return geometry; }
    bool hasGeometry() const { return !geometry.vertices.empty() && !geometry.indices.empty(); };
    PrimitiveType getPrimitiveType() const override { return PrimitiveType::Lines; };

private:
    LineGeometry geometry;
};
}
