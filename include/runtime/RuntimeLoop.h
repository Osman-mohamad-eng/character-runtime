#pragma once

#include "runtime/Scene.h"

namespace runtime {

class RuntimeLoop {
public:
    std::vector<CollisionPair> tick(Scene& scene, float deltaSeconds) const;
};

} // namespace runtime
