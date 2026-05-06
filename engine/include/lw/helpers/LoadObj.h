#pragma once

#include <lw/scene/Mesh.h>

#include <string>

namespace lw
{
    Mesh loadObj(const std::string& filename);
    Mesh loadObj(const std::string& filename, const std::string& texturePath);
}
