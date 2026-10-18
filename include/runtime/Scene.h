#pragma once

#include "runtime/Character.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace runtime {

struct CollisionPair {
    Entity* first = nullptr;
    Entity* second = nullptr;
};

class Scene {
public:
    Entity& createEntity(std::string name);
    Character& createCharacter(std::string name);

    const std::vector<std::unique_ptr<Entity>>& entities() const noexcept { return entities_; }
    std::size_t entityCount() const noexcept { return entities_.size(); }

    void update(float deltaSeconds);
    void updateWorldTransforms() const;
    void invalidateTransforms() noexcept;
    std::vector<CollisionPair> detectCollisions() const;

private:
    std::vector<std::unique_ptr<Entity>> entities_;
};

} // namespace runtime
