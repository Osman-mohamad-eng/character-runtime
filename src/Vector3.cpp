#include "runtime/Vector3.h"

#include <cmath>
#include <stdexcept>

namespace runtime {

Vector3 Vector3::operator/(float scalar) const {
    if (scalar == 0.0F) {
        throw std::invalid_argument("Cannot divide a Vector3 by zero");
    }
    return {x_ / scalar, y_ / scalar, z_ / scalar};
}

Vector3& Vector3::operator+=(const Vector3& other) noexcept {
    x_ += other.x_;
    y_ += other.y_;
    z_ += other.z_;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) noexcept {
    x_ -= other.x_;
    y_ -= other.y_;
    z_ -= other.z_;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) noexcept {
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) {
    *this = *this / scalar;
    return *this;
}

float Vector3::magnitude() const noexcept {
    return std::sqrt(dot(*this));
}

Vector3 Vector3::normalized() const noexcept {
    const float length = magnitude();
    return length > 0.0F ? *this * (1.0F / length) : Vector3{};
}

} // namespace runtime
