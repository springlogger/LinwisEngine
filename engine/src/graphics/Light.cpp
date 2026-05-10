#include <lw/graphics/Light.h>

namespace lw
{

static float SrgbToLinear(float c)
{
    return c <= 0.04045f
        ? c / 12.92f
        : std::pow((c + 0.055f) / 1.055f, 2.4f);
}

static Vector3 UnpackSrgbColor(uint32_t color)
{
    const float r = static_cast<float>((color >> 16) & 0xFF) / 255.0f;
    const float g = static_cast<float>((color >> 8) & 0xFF) / 255.0f;
    const float b = static_cast<float>(color & 0xFF) / 255.0f;

    return Vector3(
        SrgbToLinear(r),
        SrgbToLinear(g),
        SrgbToLinear(b)
    );
}

static Vector3 MultiplyColors(const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.x * b.x,
        a.y * b.y,
        a.z * b.z
    );
}

static Vector3 ClampColor(const Vector3& color)
{
    return Vector3(
        std::clamp(color.x, 0.0f, 1.0f),
        std::clamp(color.y, 0.0f, 1.0f),
        std::clamp(color.z, 0.0f, 1.0f)
    );
}

/*
* pixelColorLinear = texture pixel color converted from sRGB to linear.
*/
Vector3 DirectionalLight::applyLight(
    const Vector3& cameraPosition,
    const Vector3& worldPosition,
    const Vector3& normal,
    const Vector3& pixelColorLinear) const
{
    const Vector3 lightDir = (direction * -1.0f).normalized();
    const Vector3 viewDir = (cameraPosition - worldPosition).normalized();
    const Vector3 halfDir = (lightDir + viewDir).normalized();
    const Vector3 lightColorLinear = UnpackSrgbColor(lightColor);

    const float diffuse = std::max(0.0f, normal.dot(lightDir));

    float specular = 0.0f;
    if (diffuse > 0.0f) {
        specular = std::pow(
            std::max(0.0f, normal.dot(halfDir)),
            shininess
        ) * specularStrength;
    }

    const Vector3 ambientColor = pixelColorLinear * ambient;
    const Vector3 diffuseColor =
        MultiplyColors(pixelColorLinear, lightColorLinear) *
        (diffuse * diffuseStrength);
    const Vector3 specularColor = lightColorLinear * specular;

    return ClampColor(ambientColor + diffuseColor + specularColor);
}

} // namespace lw
