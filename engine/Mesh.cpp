#include "Mesh.h"

namespace lw
{

Mesh::Mesh() : Object3D(), geometry(), material() {}

Mesh::Mesh(Geometry geometry, Material material) : Object3D(), geometry(geometry), material(material) {}

}