#include <lw/scene/Mesh.h>
#include <lw/scene/MeshGeometry.h>

namespace lw
{

Mesh::Mesh() : RenderableObject(), geometry(), material() {}

Mesh::Mesh(MeshGeometry geometry, Material material) : RenderableObject(), geometry(geometry), material(material) {}

}