# C++ 3D Character Customization Runtime

A compact, production-style C++17 project demonstrating the foundations of a
real-time 3D character and scene runtime. The code favors explicit ownership,
small understandable classes, deterministic math, and focused automated tests.

## Features

- `Vector3` arithmetic, normalization, dot/cross products, and magnitude.
- Row-major `Matrix4` transformations, matrix composition, point/vector
  transforms, and inversion.
- Translation, Euler rotation (radians), and scale with cached hierarchical
  local/world transforms, coordinate conversion, cycle checks, and safe detach.
- Scene entities with updateable modular components.
- Configurable character body dimensions and RGB skin-color parameters.
- A runtime tick that updates components, resolves transforms, and reports
  bounding-sphere overlaps.
- A transform update profiler and configurable many-entity benchmark scene.
- GoogleTest unit tests for math, transforms, customization, profiling
  validation, and collisions.

See [docs/architecture.md](docs/architecture.md) for details and design choices.

## Project layout

```text
include/runtime/  Public math, transform, entity, and scene APIs
src/              Runtime implementations
tests/            GoogleTest unit tests
benchmark/        Many-entity transform profiling executable
docs/             Architecture notes
```

## Architecture at a glance

`RuntimeLoop` drives a `Scene`. A scene owns entities; each entity owns its
transform and components. `Character` specializes an entity with an appearance
component, which validates customization parameters and applies body dimensions
to its transform. Transforms cache local and world matrices and propagate
invalidation to descendants. Optional local-space spheres are transformed to
world space for the scene's simple collision pass.

The collision implementation is intentionally O(n^2) and serves as an
explainable baseline. For larger gameplay scenes, a broad-phase spatial hash,
BVH, or grid would reduce candidate pairs. Renderer, mesh, and animation
integration are clean follow-on component responsibilities rather than
mocked-out functionality in this project.

## Build

Requirements: CMake 3.16+, a C++17 compiler, and network access on the first
configure to fetch GoogleTest v1.14.0. Configure and build from the project
root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

On Windows, a Visual Studio C++ workload or another C++17 toolchain is needed.

## Tests

GoogleTest is fetched and linked by CMake when `BUILD_TESTING` is enabled
(default):

```sh
ctest --test-dir build -C Release --output-on-failure
```

Set `-DBUILD_TESTING=OFF` to build the runtime and benchmark without fetching
GoogleTest.

## Performance profiling

Run the benchmark with optional entity and iteration counts (defaults: 10,000
entities and 20 measured updates):

```sh
./build/character_runtime_benchmark 10000 20
```

In PowerShell, run:

```powershell
.\build\character_runtime_benchmark.exe 10000 20
```

Visual Studio multi-configuration builds place the executable under the
configuration directory, for example `build/Release/character_runtime_benchmark.exe`.
The profiler invalidates cached world matrices before each sample and reports
average, maximum, and total measured transform-update time. Results depend on
hardware, compiler, and build configuration; compare changes using the same
machine and settings.

## Relevance to real-time and game systems

The project exercises the kinds of fundamentals used in game and sports
technology work: coordinate spaces, hierarchical transforms, component-based
runtime data, character parameter validation, per-frame updates, collision
bounds, profiling, and automated regression tests. The implementation is
deliberately small enough to explain in an interview while showing clear paths
to renderer/animation integration, broad-phase collision detection, and
parallel update scheduling.

## License

This educational project does not bundle third-party source. GoogleTest is
downloaded at configure time and remains subject to its own license.
