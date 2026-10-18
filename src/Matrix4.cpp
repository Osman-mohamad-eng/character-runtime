#include "runtime/Matrix4.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace runtime {
namespace {

constexpr std::size_t index(std::size_t row, std::size_t column) noexcept {
    return row * 4 + column;
}

} // namespace

Matrix4 Matrix4::identity() noexcept {
    Matrix4 result;
    for (std::size_t i = 0; i < 4; ++i) {
        result(i, i) = 1.0F;
    }
    return result;
}

Matrix4 Matrix4::translation(const Vector3& value) noexcept {
    Matrix4 result = identity();
    result(0, 3) = value.x();
    result(1, 3) = value.y();
    result(2, 3) = value.z();
    return result;
}

Matrix4 Matrix4::scale(const Vector3& value) noexcept {
    Matrix4 result = identity();
    result(0, 0) = value.x();
    result(1, 1) = value.y();
    result(2, 2) = value.z();
    return result;
}

Matrix4 Matrix4::rotationX(float radians) noexcept {
    Matrix4 result = identity();
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);
    result(1, 1) = cosine;
    result(1, 2) = -sine;
    result(2, 1) = sine;
    result(2, 2) = cosine;
    return result;
}

Matrix4 Matrix4::rotationY(float radians) noexcept {
    Matrix4 result = identity();
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);
    result(0, 0) = cosine;
    result(0, 2) = sine;
    result(2, 0) = -sine;
    result(2, 2) = cosine;
    return result;
}

Matrix4 Matrix4::rotationZ(float radians) noexcept {
    Matrix4 result = identity();
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);
    result(0, 0) = cosine;
    result(0, 1) = -sine;
    result(1, 0) = sine;
    result(1, 1) = cosine;
    return result;
}

float Matrix4::operator()(std::size_t row, std::size_t column) const noexcept {
    return values_[index(row, column)];
}

float& Matrix4::operator()(std::size_t row, std::size_t column) noexcept {
    return values_[index(row, column)];
}

Matrix4 Matrix4::operator*(const Matrix4& other) const noexcept {
    Matrix4 result;
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            for (std::size_t inner = 0; inner < 4; ++inner) {
                result(row, column) += (*this)(row, inner) * other(inner, column);
            }
        }
    }
    return result;
}

Vector3 Matrix4::transformPoint(const Vector3& point) const {
    const float x = (*this)(0, 0) * point.x() + (*this)(0, 1) * point.y()
        + (*this)(0, 2) * point.z() + (*this)(0, 3);
    const float y = (*this)(1, 0) * point.x() + (*this)(1, 1) * point.y()
        + (*this)(1, 2) * point.z() + (*this)(1, 3);
    const float z = (*this)(2, 0) * point.x() + (*this)(2, 1) * point.y()
        + (*this)(2, 2) * point.z() + (*this)(2, 3);
    const float w = (*this)(3, 0) * point.x() + (*this)(3, 1) * point.y()
        + (*this)(3, 2) * point.z() + (*this)(3, 3);

    if (std::abs(w) < 1.0e-7F) {
        throw std::domain_error("Point transformed to an invalid homogeneous coordinate");
    }
    return {x / w, y / w, z / w};
}

Vector3 Matrix4::transformVector(const Vector3& vector) const noexcept {
    return {
        (*this)(0, 0) * vector.x() + (*this)(0, 1) * vector.y() + (*this)(0, 2) * vector.z(),
        (*this)(1, 0) * vector.x() + (*this)(1, 1) * vector.y() + (*this)(1, 2) * vector.z(),
        (*this)(2, 0) * vector.x() + (*this)(2, 1) * vector.y() + (*this)(2, 2) * vector.z()
    };
}

Matrix4 Matrix4::inverse() const {
    double augmented[4][8]{};
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            augmented[row][column] = (*this)(row, column);
        }
        augmented[row][row + 4] = 1.0;
    }

    for (std::size_t column = 0; column < 4; ++column) {
        std::size_t pivot = column;
        for (std::size_t row = column + 1; row < 4; ++row) {
            if (std::abs(augmented[row][column]) > std::abs(augmented[pivot][column])) {
                pivot = row;
            }
        }
        if (std::abs(augmented[pivot][column]) < 1.0e-12) {
            throw std::domain_error("Cannot invert a singular Matrix4");
        }
        if (pivot != column) {
            for (std::size_t entry = 0; entry < 8; ++entry) {
                std::swap(augmented[pivot][entry], augmented[column][entry]);
            }
        }

        const double divisor = augmented[column][column];
        for (double& entry : augmented[column]) {
            entry /= divisor;
        }
        for (std::size_t row = 0; row < 4; ++row) {
            if (row == column) {
                continue;
            }
            const double factor = augmented[row][column];
            for (std::size_t entry = 0; entry < 8; ++entry) {
                augmented[row][entry] -= factor * augmented[column][entry];
            }
        }
    }

    Matrix4 result;
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            result(row, column) = static_cast<float>(augmented[row][column + 4]);
        }
    }
    return result;
}

} // namespace runtime
