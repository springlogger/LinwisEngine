#pragma once

#include <lw/graphics/Texture.h>
#include <string>

namespace lw
{

class Material {
public:
    Material() : texture() {}
    explicit Material(const std::string& texturePath) : texture(texturePath) {}
    explicit Material(Texture tex) : texture(std::move(tex)) {}

    const Texture& getTexture() const { return texture; }
    void setTexture(Texture tex)           { texture = std::move(tex); }
    void setTexture(const std::string& path) { texture = Texture(path); }

private:
    Texture texture;
};

} // namespace lw
