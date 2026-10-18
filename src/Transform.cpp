#include "runtime/Transform.h"

#include <stdexcept>

namespace runtime {

Transform::~Transform() {
    if (parent_ != nullptr) {
        parent_->removeChild(this);
    }
    for (Transform* child : children_) {
        child->parent_ = nullptr;
        child->markDirty();
    }
}

void Transform::setLocalPosition(const Vector3& position) noexcept {
    localPosition_ = position;
    localDirty_ = true;
    markDirty();
}

void Transform::setLocalRotation(const Vector3& radians) noexcept {
    localRotation_ = radians;
    localDirty_ = true;
    markDirty();
}

void Transform::setLocalScale(const Vector3& scale) noexcept {
    localScale_ = scale;
    localDirty_ = true;
    markDirty();
}

void Transform::setParent(Transform* parent) {
    if (parent == this) {
        throw std::invalid_argument("A Transform cannot be its own parent");
    }
    for (Transform* ancestor = parent; ancestor != nullptr; ancestor = ancestor->parent_) {
        if (ancestor == this) {
            throw std::invalid_argument("Transform parenting cannot create a cycle");
        }
    }
    if (parent_ == parent) {
        return;
    }
    if (parent != nullptr) {
        parent->children_.push_back(this);
    }
    if (parent_ != nullptr) {
        parent_->removeChild(this);
    }
    parent_ = parent;
    markDirty();
}

const Matrix4& Transform::localMatrix() const noexcept {
    if (localDirty_) {
        localMatrix_ = Matrix4::translation(localPosition_)
            * Matrix4::rotationZ(localRotation_.z())
            * Matrix4::rotationY(localRotation_.y())
            * Matrix4::rotationX(localRotation_.x())
            * Matrix4::scale(localScale_);
        localDirty_ = false;
    }
    return localMatrix_;
}

const Matrix4& Transform::worldMatrix() const {
    if (worldDirty_) {
        worldMatrix_ = parent_ == nullptr ? localMatrix() : parent_->worldMatrix() * localMatrix();
        worldDirty_ = false;
    }
    return worldMatrix_;
}

Vector3 Transform::worldPosition() const {
    return worldMatrix().transformPoint({});
}

Vector3 Transform::localToWorld(const Vector3& point) const {
    return worldMatrix().transformPoint(point);
}

Vector3 Transform::worldToLocal(const Vector3& point) const {
    return worldMatrix().inverse().transformPoint(point);
}

void Transform::invalidateWorldMatrix() noexcept {
    markDirty();
}

void Transform::markDirty() noexcept {
    if (worldDirty_) {
        return;
    }
    worldDirty_ = true;
    for (Transform* child : children_) {
        child->markDirty();
    }
}

void Transform::removeChild(Transform* child) noexcept {
    for (auto iterator = children_.begin(); iterator != children_.end(); ++iterator) {
        if (*iterator == child) {
            children_.erase(iterator);
            return;
        }
    }
}

} // namespace runtime
