#pragma once

#include <lw/scene/Object3D.h>
#include <lw/math/Matrix4.h>
#include <lw/scene/MeshGeometry.h>
#include <lw/scene/Material.h>
#include <lw/graphics/RenderableObject.h>

#include <string>

namespace lw
{

class Mesh : public RenderableObject {
public:
    Mesh();
    Mesh(MeshGeometry geometry, Material material);

    bool hasGeometry() const { return !geometry.vertices.empty() && !geometry.indices.empty(); }

    const MeshGeometry& getGeometry() const { return geometry; }
    const Material&     getMaterial() const { return material; }

    PrimitiveType getPrimitiveType() const override { return PrimitiveType::Triangles; }

    void setMaterial(Material mat);
    void setTexture(const std::string& path);
    void setTexture(Texture texture);

protected:
    MeshGeometry geometry;
    Material     material;
};

} // namespace lw
