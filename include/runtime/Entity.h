#pragma once

#include "runtime/BoundingSphere.h"
#include "runtime/Component.h"

#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace runtime {

class Entity {
public:
    explicit Entity(std::string name);
    virtual ~Entity() = default;

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    const std::string& name() const noexcept { return name_; }
    Transform& transform() noexcept { return transform_; }
    const Transform& transform() const noexcept { return transform_; }

    template <typename T, typename... Args>
    T& addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T& result = *component;
        component->owner_ = this;
        components_.push_back(std::move(component));
        return result;
    }

    void setBoundingSphere(const BoundingSphere& sphere) { bounds_ = sphere; }
    void clearBoundingSphere() noexcept { bounds_.reset(); }
    const std::optional<BoundingSphere>& boundingSphere() const noexcept { return bounds_; }
    std::optional<BoundingSphere> worldBoundingSphere() const;

    virtual void update(float deltaSeconds);

private:
    std::string name_;
    Transform transform_;
    std::vector<std::unique_ptr<Component>> components_;
    std::optional<BoundingSphere> bounds_;
};

} // namespace runtime
