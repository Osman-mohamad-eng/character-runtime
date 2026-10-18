#include "runtime/Entity.h"

#include <stdexcept>

namespace runtime {

Entity::Entity(std::string name) : name_(std::move(name)) {
    if (name_.empty()) {
        throw std::invalid_argument("Entity name cannot be empty");
    }
}

std::optional<BoundingSphere> Entity::worldBoundingSphere() const {
    if (!bounds_) {
        return std::nullopt;
    }
    return bounds_->transformed(transform_);
}

void Entity::update(float deltaSeconds) {
    for (const auto& component : components_) {
        component->update(deltaSeconds);
    }
}

} // namespace runtime
