#include "runtime/TransformProfiler.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

std::size_t parseCount(const char* value, const char* label) {
    try {
        const std::string text(value);
        std::size_t parsed = 0;
        const unsigned long long count = std::stoull(text, &parsed);
        if (parsed != text.size() || count == 0) {
            throw std::invalid_argument("value must be a positive integer");
        }
        return static_cast<std::size_t>(count);
    } catch (const std::exception& error) {
        throw std::invalid_argument(std::string(label) + ": " + error.what());
    }
}

} // namespace

int main(int argc, char** argv) {
    try {
        const std::size_t entityCount = argc > 1 ? parseCount(argv[1], "Entity count") : 10000;
        const std::size_t iterations = argc > 2 ? parseCount(argv[2], "Iteration count") : 20;

        runtime::Scene scene;
        for (std::size_t index = 0; index < entityCount; ++index) {
            runtime::Entity& entity = scene.createEntity("benchmark-entity-" + std::to_string(index));
            const float x = static_cast<float>(index % 100);
            const float y = static_cast<float>((index / 100) % 100);
            const float z = static_cast<float>(index / 10000);
            entity.transform().setLocalPosition({x, y, z});
            entity.setBoundingSphere({{}, 0.5F});
        }

        const runtime::TransformProfiler profiler;
        const runtime::ProfileResult result = profiler.measure(scene, iterations);
        std::cout << "C++ 3D Character Customization Runtime benchmark\n"
                  << "Entities: " << entityCount << "\n"
                  << "Transform update iterations: " << result.iterations << "\n"
                  << std::fixed << std::setprecision(4)
                  << "Average transform update: " << result.averageMilliseconds << " ms\n"
                  << "Maximum transform update: " << result.maximumMilliseconds << " ms\n"
                  << "Total measured time: " << result.totalMilliseconds << " ms\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "Benchmark error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
