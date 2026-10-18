#include "runtime/RuntimeLoop.h"

namespace runtime {

std::vector<CollisionPair> RuntimeLoop::tick(Scene& scene, float deltaSeconds) const {
    scene.update(deltaSeconds);
    return scene.detectCollisions();
}

} // namespace runtime
