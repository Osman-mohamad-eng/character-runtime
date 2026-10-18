#pragma once

#include "runtime/Vector3.h"

#include <array>
#include <cstddef>

namespace runtime {

class Matrix4 {
public:
    Matrix4() noexcept = default;

    static Matrix4 identity() noexcept;
    static Matrix4 translation(const Vector3& value) noexcept;
    static Matrix4 scale(const Vector3& value) noexcept;
    static Matrix4 rotationX(float radians) noexcept;
    static Matrix4 rotationY(float radians) noexcept;
    static Matrix4 rotationZ(float radians) noexcept;

    float operator()(std::size_t row, std::size_t column) const noexcept;
    float& operator()(std::size_t row, std::size_t column) noexcept;

    Matrix4 operator*(const Matrix4& other) const noexcept;
    Vector3 transformPoint(const Vector3& point) const;
    Vector3 transformVector(const Vector3& vector) const noexcept;
    Matrix4 inverse() const;

private:
    std::array<float, 16> values_{};
};

} // namespace runtime
