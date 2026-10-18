# Project guide

## Architecture

- `include/runtime/` contains the public runtime API; `src/` contains
  implementation files.
- `Vector3` and `Matrix4` form the math layer. Matrices are row-major and use
  column-vector transforms.
- `Transform` owns local TRS state, tracks non-owning parent/child links, and
  lazily caches local and world matrices.
- `Entity` owns a transform and updateable `Component` instances.
  `CharacterAppearanceComponent` validates and applies `CharacterParameters`.
- `Scene` owns entities, updates components and transforms, and performs
  pairwise bounding-sphere checks. `RuntimeLoop` exposes a single tick.
- `TransformProfiler` measures transform-cache recomputation;
  `benchmark/main.cpp` builds the many-entity workload.
- `tests/` contains GoogleTest coverage; `docs/architecture.md` describes
  implementation boundaries and extension points.

## Coding conventions

- Require C++17 and keep public declarations in `include/runtime/*.h`.
- Use the `runtime` namespace, descriptive names, and four-space indentation.
- Prefer value types for math data and `std::unique_ptr` for owned entities and
  components. Transform parent/child links are non-owning.
- Report invalid input with standard exceptions rather than silently accepting
  invalid state. Keep component behavior in `update` and validate externally
  supplied character parameters before applying them.
- Keep comments limited to behavior that is not clear from names and types.
- Add or update focused GoogleTests for behavior changes; build and run CTest
  before considering a change complete.
- Do not add rendering, asset, or platform dependencies to the core runtime
  without a concrete use case.
