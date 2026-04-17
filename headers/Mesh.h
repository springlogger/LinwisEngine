#pragma once

#include "Object3D.h"
#include "Matrix4.h"
#include "Geometry.h"
#include "Material.h"

namespace lw 
{

class Mesh : public Object3D {

public:
  Mesh();
  Mesh(Geometry geometry, Material material);

  bool hasGeometry() const override { return !geometry.vertices.empty() && !geometry.indices.empty(); };

  const Geometry& getGeometry() const { return geometry; };
  const Material& getMaterial() const { return material; };

protected:

  Geometry geometry;
  Material material;

};

}