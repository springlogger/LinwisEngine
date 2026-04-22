#pragma once

#include <lw/scene/Object3D.h>

namespace lw
{

  enum class PrimitiveType
  {
      Triangles,
      Lines
  };

  class RenderableObject : public Object3D
  {
  public:
      virtual ~RenderableObject() = default;

      virtual PrimitiveType getPrimitiveType() const = 0;
  };
} // namespace lw
