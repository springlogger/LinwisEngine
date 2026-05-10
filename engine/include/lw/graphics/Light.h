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

    /*
    * pixelColorLinear = texture pixel color converted from sRGB to linear.
    */
    Vector3 applyLight(const Vector3& cameraPosition, const Vector3& worldPosition,  const Vector3& normal, const Vector3& pixelColorLinear) const;
};


} // namespace lw
