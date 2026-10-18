#include "runtime/TransformProfiler.h"

#include <algorithm>
#include <stdexcept>

namespace runtime {

ProfileResult TransformProfiler::measure(Scene& scene, std::size_t iterations) const {
    if (iterations == 0) {
        throw std::invalid_argument("Transform profiling requires at least one iteration");
    }

    ProfileResult result;
    result.iterations = iterations;
    for (std::size_t iteration = 0; iteration < iterations; ++iteration) {
        scene.invalidateTransforms();
        const auto start = std::chrono::steady_clock::now();
        scene.updateWorldTransforms();
        const auto finish = std::chrono::steady_clock::now();
        const double elapsed = std::chrono::duration<double, std::milli>(finish - start).count();
        result.totalMilliseconds += elapsed;
        result.maximumMilliseconds = std::max(result.maximumMilliseconds, elapsed);
    }
    result.averageMilliseconds = result.totalMilliseconds / static_cast<double>(iterations);
    return result;
}

} // namespace runtime
