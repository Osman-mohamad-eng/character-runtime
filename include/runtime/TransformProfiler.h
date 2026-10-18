#pragma once

#include "runtime/Scene.h"

#include <chrono>
#include <cstddef>

namespace runtime {

struct ProfileResult {
    std::size_t iterations = 0;
    double totalMilliseconds = 0.0;
    double averageMilliseconds = 0.0;
    double maximumMilliseconds = 0.0;
};

class TransformProfiler {
public:
    ProfileResult measure(Scene& scene, std::size_t iterations) const;
};

} // namespace runtime
