#include <lw/graphics/Light.h>

namespace lw
{
    uint32_t DirectionalLight::AddSpecular(uint32_t baseColor, uint32_t lightColor, float specular)
    {
        auto add = [specular](uint32_t base, uint32_t light) {
            float value = static_cast<float>(base) +
                        static_cast<float>(light) * specular;

            return static_cast<uint32_t>(
                std::clamp(value, 0.0f, 255.0f)
            );
        };

        uint32_t br = (baseColor >> 16) & 0xFF;
        uint32_t bg = (baseColor >> 8) & 0xFF;
        uint32_t bb = baseColor & 0xFF;

        uint32_t lr = (lightColor >> 16) & 0xFF;
        uint32_t lg = (lightColor >> 8) & 0xFF;
        uint32_t lb = lightColor & 0xFF;

        return (add(br, lr) << 16) |
            (add(bg, lg) << 8) |
                add(bb, lb);
    }

    uint32_t DirectionalLight::MultiplyColor(uint32_t color, float factor)
    {
        auto scale = [factor](uint32_t c) {
            return static_cast<uint32_t>(
                std::clamp(c * factor, 0.0f, 255.0f)
            );
        };

        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >> 8) & 0xFF;
        uint32_t b = color & 0xFF;

        return (scale(r) << 16) | (scale(g) << 8) | scale(b);
    }

    /*
    * pixelColor = texturePixelColor or pixelColorWithoutLight
    */
    uint32_t DirectionalLight::applyLight(const Vector3& cameraPosition, const Vector3& worldPosition,  const Vector3& normal, uint32_t pixelColor) const {
        Vector3 lightDir = (direction * -1.0f).normalized();
        Vector3 viewDir = (cameraPosition - worldPosition).normalized();
        Vector3 halfDir = (lightDir + viewDir).normalized();

        float diffuse = std::max(0.0f, normal.dot(lightDir));
        float specular = 0.0f;
        if (diffuse > 0.0f) {
            specular = std::pow(
                std::max(0.0f, normal.dot(halfDir)),
                shininess
            ) * specularStrength;
        }

        float lighting = std::clamp(
            ambient + diffuse * diffuseStrength,
            0.0f,
            1.0f
        );

        uint32_t litDiffuseColor = MultiplyColor(pixelColor, lighting);
        uint32_t finalColor = AddSpecular(litDiffuseColor, lightColor, specular);

        return finalColor;
    }
} // namespace lw
