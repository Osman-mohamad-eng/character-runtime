#include "runtime/Scene.h"

#include <cmath>
#include <stdexcept>

namespace runtime {

Entity& Scene::createEntity(std::string name) {
    auto entity = std::make_unique<Entity>(std::move(name));
    Entity& result = *entity;
    entities_.push_back(std::move(entity));
    return result;
}

Character& Scene::createCharacter(std::string name) {
    auto character = std::make_unique<Character>(std::move(name));
    Character& result = *character;
    entities_.push_back(std::move(character));
    return result;
}

void Scene::update(float deltaSeconds) {
    if (!std::isfinite(deltaSeconds) || deltaSeconds < 0.0F) {
        throw std::invalid_argument("Scene delta time must be finite and non-negative");
    }
    for (const auto& entity : entities_) {
        entity->update(deltaSeconds);
    }
    updateWorldTransforms();
}

void Scene::updateWorldTransforms() const {
    for (const auto& entity : entities_) {
        (void)entity->transform().worldMatrix();
    }
}

void Scene::invalidateTransforms() noexcept {
    for (const auto& entity : entities_) {
        entity->transform().invalidateWorldMatrix();
    }
}

std::vector<CollisionPair> Scene::detectCollisions() const {
    std::vector<CollisionPair> collisions;
    for (std::size_t firstIndex = 0; firstIndex < entities_.size(); ++firstIndex) {
        const auto& first = entities_[firstIndex];
        const auto firstBounds = first->worldBoundingSphere();
        if (!firstBounds) {
            continue;
        }
        for (std::size_t secondIndex = firstIndex + 1; secondIndex < entities_.size(); ++secondIndex) {
            const auto& second = entities_[secondIndex];
            const auto secondBounds = second->worldBoundingSphere();
            if (secondBounds && firstBounds->intersects(*secondBounds)) {
                collisions.push_back({first.get(), second.get()});
            }
        }
    }
    return collisions;
}

} // namespace runtime
