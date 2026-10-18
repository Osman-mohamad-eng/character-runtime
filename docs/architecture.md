# Architecture

## Runtime flow

`RuntimeLoop::tick` validates and advances a `Scene`. Each `Entity` updates its
owned components; the scene then resolves cached world matrices and performs a
simple pairwise bounding-sphere collision pass. The return value is a list of
colliding entity pairs, leaving response policy to the caller.

## Math and transforms

`Vector3` provides the common three-dimensional operations used by the runtime.
`Matrix4` uses row-major storage and column-vector math. Transform composition
is `translation * rotationZ * rotationY * rotationX * scale`, with Euler angles
in radians. A child world matrix is its parent's world matrix multiplied by its
local matrix. Setters invalidate cached matrices down the hierarchy; reading a
world matrix lazily recomputes only dirty transforms. Parent links are
non-owning, cycles are rejected, and children detach if their parent is
destroyed.

`localToWorld` and `worldToLocal` use the composed world matrix and its inverse.
Trying to invert a singular transform is reported as a `std::domain_error`.

## Entities and character customization

`Entity` owns its transform and a list of independently updateable `Component`
objects. `Character` is an entity with a `CharacterAppearanceComponent`.
`CharacterParameters` configures positive body dimensions and normalized RGB
skin color; the appearance component applies dimensions as the entity's local
scale. The color is retained as runtime customization data for a renderer or
material system to consume.

Entities may optionally carry local-space `BoundingSphere` bounds. The scene
transforms their centers and scales their radii before testing overlap. This
pairwise collision pass is intentionally straightforward and has quadratic
complexity; it is a useful baseline for introducing spatial partitioning.

## Profiling and extension points

`TransformProfiler` invalidates scene transform caches and records steady-clock
timings for world-transform updates. The benchmark executable creates a
configurable grid of entities and reports average, maximum, and total update
time. These results are machine- and build-configuration-dependent; use them
for comparisons on the same machine, not as cross-platform guarantees.

Components are the extension point for gameplay and customization behavior.
Rendering, animation, asset loading, collision response, and multithreaded
scene scheduling are intentionally outside this small runtime.
