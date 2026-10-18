#pragma once

#include "runtime/Matrix4.h"

#include <vector>

namespace runtime {

class Transform {
public:
    Transform() = default;
    ~Transform();

    Transform(const Transform&) = delete;
    Transform& operator=(const Transform&) = delete;

    const Vector3& localPosition() const noexcept { return localPosition_; }
    const Vector3& localRotation() const noexcept { return localRotation_; }
    const Vector3& localScale() const noexcept { return localScale_; }

    void setLocalPosition(const Vector3& position) noexcept;
    void setLocalRotation(const Vector3& radians) noexcept;
    void setLocalScale(const Vector3& scale) noexcept;

    Transform* parent() const noexcept { return parent_; }
    void setParent(Transform* parent);

    const Matrix4& localMatrix() const noexcept;
    const Matrix4& worldMatrix() const;
    Vector3 worldPosition() const;
    Vector3 localToWorld(const Vector3& point) const;
    Vector3 worldToLocal(const Vector3& point) const;

    void invalidateWorldMatrix() noexcept;

private:
    void markDirty() noexcept;
    void removeChild(Transform* child) noexcept;

    Vector3 localPosition_{};
    Vector3 localRotation_{};
    Vector3 localScale_{1.0F, 1.0F, 1.0F};
    Transform* parent_ = nullptr;
    std::vector<Transform*> children_;
    mutable Matrix4 localMatrix_;
    mutable Matrix4 worldMatrix_;
    mutable bool localDirty_ = true;
    mutable bool worldDirty_ = true;
};

} // namespace runtime
