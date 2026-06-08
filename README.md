# Cpp-Physics-Engine

A 2D/3D physics engine built from scratch in C++ with SFML 3.x rendering. Implements rigid body dynamics, force decomposition, material properties, rotation, and collision resolution without any third-party physics library.

---

## Features

### 2D
- **Rigid body physics** — force-driven velocity and position integration with gravity
- **Rotation** — angular velocity, torque, moment of inertia per shape type
- **Material system** — friction (`mu`) and restitution per material type
- **Shape system** — rectangle, triangle, circle (polygon approximation)
- **Center-based coordinates** — all sprite positions track geometric center for consistent physics and rendering
- **Bounding circle collision** — rotation-safe broad phase detection with per-shape radius (exact for circle, diagonal for rectangle, centroid-based for triangle)
- **Face-normal resolution** — fixed obstacles resolve collision along the nearest surface face rather than minimum overlap axis; dynamic sprite pairs use specular velocity reflection along contact normal
- **Stable surface contact** — floor contact bleeds angular velocity via friction torque; shapes settle onto stable edges over time
- **Sponge friction** — shapes sliding along walls and floors lose velocity proportionally to material `mu`, generating rolling/spinning torque from contact
- **Fixed bodies** — static immovable shapes for platforms and walls; collision branch selected at runtime based on `fixed` state
- **Boundary collision** — per-shape-type wall bounds (circle uses radius, rectangle uses half-extents, triangle uses centroid offsets) with restitution and friction
- **Multi-obstacle resolution** — iterative solver runs up to 5 passes per frame for stable contact with multiple fixed surfaces
- **Window-aware physics** — boundaries and obstacles adapt to window resize events

### 3D
- **Perspective projection** — per-vertex projection with configurable FOV and screen center; scales toward screen center not origin
- **Painter's algorithm** — faces sorted by average Z depth, drawn back-to-front
- **Backface culling** — faces pointing away from camera skipped entirely
- **Shape system** — box, triangular prism, pyramid, sphere (lat/lon quad approximation), parallelepiped with shear
- **Per-face shading** — each face has a distinct grey shade giving depth illusion without a light source
- **3D AABB collision** — axis-aligned bounding box on all three axes with velocity-assisted axis selection for ambiguous cases
- **Depth movement** — Q/E keys move along Z axis; perspective scale changes visibly with depth

---

## Project Structure

```
Cpp-Physics-Engine/
├── main.cpp                    — entry point, SFML window, game loop, input, screen routing
├── arial.ttf                   — font for UI text
├── 2D/
│   ├── src/
│   │   └── classes.hpp         — Shape, Shapetype, Material, Materials, Forces, Position, Sprite
│   └── include/
│       ├── collisions.h        — declarations
│       └── collisions.cpp      — AABBcollision, resolveCollision, resolveAllCollisions
└── 3D/
    ├── src/
    │   ├── classes3D.hpp       — Vec3, Sprite3D
    │   ├── shape3D.hpp         — Face, Shape3D, Shapes3D namespace (box, prism, pyramid, sphere, parallelepiped)
    │   └── render3D.hpp        — project(), isFacingCamera(), drawShape3D()
    └── include/
        ├── collisions3D.h      — declarations
        └── collisions3D.cpp    — AABBcollision3D, resolveCollision3D, resolveAllCollisions3D
```

---

## Dependencies

- **SFML 3.1** — `sudo pacman -S sfml`
- **g++** with C++17 or later
- **Linux** (developed on EndeavourOS / Arch)

---

## Build

```bash
g++ main.cpp 2D/include/collisions.cpp 3D/src/collisions3D.cpp \
    -o physics -lsfml-graphics -lsfml-window -lsfml-system
```

---

## Run

```bash
./physics
```

On launch a menu appears. Press **2** for 2D simulation, **3** for 3D simulation.

---

## Controls

### 2D
| Key | Action |
|---|---|
| `2` | Enter 2D simulation |
| `←` `→` `↑` `↓` | Apply force in direction |
| `Z` | Increase angular velocity (spin) |
| `F` | Toggle fixed/dynamic on player sprite |
| `A` + `R` | Switch to rectangle |
| `A` + `T` | Switch to triangle |
| `A` + `C` | Switch to circle |

### 3D
| Key | Action |
|---|---|
| `3` | Enter 3D simulation |
| `←` `→` `↑` `↓` | Apply force X/Y |
| `Q` / `E` | Apply force toward / away (Z axis) |
| `A` + `R` | Switch to box |
| `A` + `T` | Switch to triangular prism |
| `A` + `P` | Switch to pyramid |
| `A` + `S` | Switch to sphere |
| `A` + `L` | Switch to parallelepiped |

---

## Physics Model

### Force Integration
```
acceleration = force / mass
velocity    += acceleration × deltaTime
position    += velocity × deltaTime
```
Gravity is applied unconditionally each frame as `mass × g` in the Y axis (positive Y = down in screen space).

### Rotation
```
torque            = contactX × Fy − contactY × Fx
momentOfInertia   = mass × (width² + height²) / 12
angularAccel      = torque / momentOfInertia
angularVelocity  += angularAccel × deltaTime
angle            += angularVelocity × deltaTime
```
Torque is only generated when a force is applied with a non-zero contact offset from center — keyboard input forces pass through center and generate no torque. Contact-generated torque from collision resolution causes natural rolling and spinning behavior.

### Collision Resolution

**Fixed obstacle (face-normal):**
Finds the penetrating axis by comparing shape-aware extents against obstacle half-dimensions. Resolves along the nearest face normal. Floor contact applies sponge friction bleeds and generates rolling torque. Side contact zeroes torque and damps angular velocity.

**Dynamic vs dynamic (specular reflection):**
Reflects each sprite's velocity along the contact normal vector using `v − 2(v·n)n`, weighted by material restitution.

### Materials
| Material | Friction (μ) | Restitution | State |
|---|---|---|---|
| Rubber | 0.8 | 0.7 | Elastic Solid |
| Ice | 0.005 | 0.01 | Plastic Solid |
| Wood | 0.4 | 0.35 | Elastic Solid |
| Concrete | 0.6 | 0.2 | Plastic Solid |
| Water | 1.7 | 1.0 | Viscous Liquid |
| Oil | 4.8 | 5.0 | Viscous Liquid |

### 3D Projection
```
camZ    = z + fov
p       = fov / camZ
screenX = cx + (worldX − cx) × p
screenY = cy + (worldY − cy) × p
```
Objects scale toward the screen center as Z increases. At `z = 0` objects appear at full size. Backface culling removes faces where the Z component of the surface normal faces away from camera.

---

## Planned expansions for those who want to contribute

- **SAT collision** — Separating Axis Theorem for pixel-accurate rotated shape collision
- **Rigid body constraints** — springs, hinges, ropes between sprites
- **Buoyancy** — Water and Oil materials affecting submerged sprites
- **Multiple dynamic sprites** — pairwise collision between arbitrary numbers of sprites
- **Slope movement** — inclined surface force decomposition using `position.slope`
- **State-based collision** — elastic/plastic/viscous collision types based on material pairing
- **GUI** — runtime shape spawning and material selection

---

## Notes

- `arial.ttf` must be present in the same directory as the binary at runtime
- Physics boundaries adapt automatically on `sf::Event::Resized`
- The 2D collision system uses bounding circles for broad phase — a conservative approximation that overestimates slightly at non-zero rotation angles; SAT is the correct long-term fix
- 3D physics is AABB only with no rotation — the 3D mode demonstrates rendering and depth movement, not full 3D rigid body simulation
- Gravity scale is `98.0` pixels/s² in 3D and derived from the `Forces` constructor in 2D (default `9.8`, scaled by input force magnitude)
