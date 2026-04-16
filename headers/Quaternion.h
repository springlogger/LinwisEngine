#pragma once

#include "Vector3.h"

namespace lw {

class Quaternion {
public:
    Quaternion();
    Quaternion(const Vector3& axis, float angle);

    Vector3 rotate(const Vector3& vector) const;

private:
    float x;
    float y;
    float z;
    float w;
};

} // namespace lw
