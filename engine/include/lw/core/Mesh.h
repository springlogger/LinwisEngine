#pragma once

#include <lw/core/Object3D.h>
#include <lw/math/Matrix4.h>
#include <lw/core/Geometry.h>
#include <lw/core/Material.h>

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