#include "runtime/BoundingSphere.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace runtime {

BoundingSphere::BoundingSphere(Vector3 sphereCenter, float sphereRadius)
    : center(sphereCenter), radius(sphereRadius) {
    if (!std::isfinite(radius) || radius < 0.0F) {
        throw std::invalid_argument("Bounding sphere radius must be finite and non-negative");
    }
}

BoundingSphere BoundingSphere::transformed(const Transform& transform) const {
    const Matrix4& matrix = transform.worldMatrix();
    const Vector3 xAxis = matrix.transformVector({1.0F, 0.0F, 0.0F});
    const Vector3 yAxis = matrix.transformVector({0.0F, 1.0F, 0.0F});
    const Vector3 zAxis = matrix.transformVector({0.0F, 0.0F, 1.0F});
    const float maxScale = std::max({xAxis.magnitude(), yAxis.magnitude(), zAxis.magnitude()});
    return {matrix.transformPoint(center), radius * maxScale};
}

bool BoundingSphere::intersects(const BoundingSphere& other) const noexcept {
    const float combinedRadius = radius + other.radius;
    return (center - other.center).dot(center - other.center) <= combinedRadius * combinedRadius;
}

} // namespace runtime
