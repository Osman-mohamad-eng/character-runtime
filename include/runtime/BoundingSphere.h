#pragma once

#include "runtime/Transform.h"

namespace runtime {

struct BoundingSphere {
    Vector3 center{};
    float radius = 0.5F;

    BoundingSphere() = default;
    BoundingSphere(Vector3 center, float radius);

    BoundingSphere transformed(const Transform& transform) const;
    bool intersects(const BoundingSphere& other) const noexcept;
};

} // namespace runtime
