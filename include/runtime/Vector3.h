#pragma once

namespace runtime {

class Vector3 {
public:
    constexpr Vector3() noexcept = default;
    constexpr Vector3(float x, float y, float z) noexcept : x_(x), y_(y), z_(z) {}

    constexpr float x() const noexcept { return x_; }
    constexpr float y() const noexcept { return y_; }
    constexpr float z() const noexcept { return z_; }

    constexpr Vector3 operator+(const Vector3& other) const noexcept;
    constexpr Vector3 operator-(const Vector3& other) const noexcept;
    constexpr Vector3 operator*(float scalar) const noexcept;
    Vector3 operator/(float scalar) const;

    Vector3& operator+=(const Vector3& other) noexcept;
    Vector3& operator-=(const Vector3& other) noexcept;
    Vector3& operator*=(float scalar) noexcept;
    Vector3& operator/=(float scalar);

    constexpr float dot(const Vector3& other) const noexcept;
    constexpr Vector3 cross(const Vector3& other) const noexcept;
    float magnitude() const noexcept;
    Vector3 normalized() const noexcept;

    friend constexpr Vector3 operator*(float scalar, const Vector3& vector) noexcept {
        return vector * scalar;
    }

private:
    float x_ = 0.0F;
    float y_ = 0.0F;
    float z_ = 0.0F;
};

constexpr Vector3 Vector3::operator+(const Vector3& other) const noexcept {
    return {x_ + other.x_, y_ + other.y_, z_ + other.z_};
}

constexpr Vector3 Vector3::operator-(const Vector3& other) const noexcept {
    return {x_ - other.x_, y_ - other.y_, z_ - other.z_};
}

constexpr Vector3 Vector3::operator*(float scalar) const noexcept {
    return {x_ * scalar, y_ * scalar, z_ * scalar};
}

constexpr float Vector3::dot(const Vector3& other) const noexcept {
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

constexpr Vector3 Vector3::cross(const Vector3& other) const noexcept {
    return {
        y_ * other.z_ - z_ * other.y_,
        z_ * other.x_ - x_ * other.z_,
        x_ * other.y_ - y_ * other.x_
    };
}

} // namespace runtime
