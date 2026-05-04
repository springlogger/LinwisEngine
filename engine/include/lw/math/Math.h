#pragma once

namespace lw::math {

constexpr float pi = 3.14159265358979323846f;

constexpr float degToRad(float degrees) {
    return degrees * pi / 180.0f;
}

} // namespace lw::math
