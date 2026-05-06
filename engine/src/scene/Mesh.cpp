#include <lw/scene/Mesh.h>
#include <lw/scene/MeshGeometry.h>

namespace lw
{

Mesh::Mesh() : RenderableObject(), geometry(), material() {}

Mesh::Mesh(MeshGeometry geom, Material mat)
    : RenderableObject(), geometry(std::move(geom)), material(std::move(mat)) {}

void Mesh::setMaterial(Material mat) {
    material = std::move(mat);
}

void Mesh::setTexture(const std::string& path) {
    material.setTexture(path);
}

void Mesh::setTexture(Texture tex) {
    material.setTexture(std::move(tex));
}

} // namespace lw
