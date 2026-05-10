#pragma once

#include <lw/math/Vector3.h>

#include <cstdint>
#include <algorithm>
#include <cmath>

namespace lw {

class DirectionalLight {
public:
    Vector3 direction = Vector3(0.0f, -1.0f, 0.0f);
    uint32_t lightColor = 0x00FFFFFF;
    float ambient = 0.2f; 
    float diffuseStrength = 0.8f;
    float specularStrength = 0.5f;
    float shininess = 32.0f;

    static uint32_t AddSpecular(uint32_t baseColor, uint32_t lightColor, float specular);

    static uint32_t MultiplyColor(uint32_t color, float factor);

    /*
    * pixelColor = texturePixelColor or pixelColorWithoutLight
    */
    uint32_t applyLight(const Vector3& cameraPosition, const Vector3& worldPosition,  const Vector3& normal, uint32_t pixelColor) const;
};


} // namespace lw
