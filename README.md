# Cpp-Physics-Engine

A 2D physics engine built from scratch in C++ with SFML 3.x rendering. Implements rigid body dynamics, force decomposition, material properties, and AABB collision detection without any third-party physics library.

---

## Features

- **Rigid body physics** — force-driven velocity and position integration
- **Material system** — friction (`mu`) and restitution per material
- **Shape system** — rectangle, triangle, circle (polygon approximation)
- **AABB collision detection** — axis-aligned bounding box sprite-vs-sprite
- **Fixed bodies** — static immovable shapes for platforms and walls
- **Boundary collision** — floor, ceiling, and wall bounce with restitution
- **External force application** — push sprites with keyboard input
- **Window-aware physics** — boundaries adapt to window size and resize events
- **SFML rendering** — convex polygon rendering from vertex data

---

## Project Structure

```
Cpp-Physics-Engine/
├── main.cpp               — entry point, SFML window, game loop, input
├── arial.ttf              — font for UI text
├── 2D/
│   ├── src/
│   │   ├── classes.hpp    — Shape, Material, Forces, Position, Sprite
│   │   └── collisions.hpp — AABBcollision, resolveCollision
│   └── include/
└── 3D/                    — planned
    ├── src/
    └── include/
```

---

## Dependencies

- **SFML 3.1** — `sudo pacman -S sfml`
- **g++** with C++17 or later
- **Linux** (developed on EndeavourOS / Arch)

---

## Build

```bash
g++ main.cpp -o physics -lsfml-graphics -lsfml-window -lsfml-system
```

---

## Run

```bash
./physics
```

On launch, a menu appears. Press **2** to enter the 2D simulation.

---

## Controls

| Key | Action |
|---|---|
| `2` | Enter 2D simulation |
| `3` | Enter 3D simulation (planned) |
| `←` | Apply force left |
| `→` | Apply force right |
| `↑` | Apply force up |

---

## Physics Model

### Force Integration
Each frame, net force is computed and integrated into velocity, then position:

```
acceleration = force / mass
velocity    += acceleration * deltaTime
position    += velocity * deltaTime
```

### Gravity
Gravity acts downward in SFML screen space (positive y). Default scale is `980.0` pixels/s² to produce visible motion at pixel resolution.

### Materials

| Material | Friction (μ) | Restitution |
|---|---|---|
| Rubber | 0.8 | 0.7 |
| Ice | 0.05 | 0.1 |
| Wood | 0.4 | 0.35 |
| Concrete | 0.6 | 0.2 |

### Collision
- **Boundary** — sprites bounce off all four window edges with restitution damping
- **AABB** — axis-aligned bounding box checks between sprites; velocity exchange weighted by mass and restitution
- **Fixed bodies** — `fixed = true` skips force integration; dynamic sprites bounce off them

---

## Planned

- 3D simulation mode
- SAT collision detection for rotated and triangular shapes
- Multiple simultaneous sprites with pairwise collision
- Slope-based movement and inclined surfaces
- GUI for spawning shapes and selecting materials at runtime

---

## Notes

- Font (`arial.ttf`) must be in the same directory as the binary at runtime
- Physics boundaries adapt automatically to window resize via `sf::Event::Resized`
- `classes.hpp` and `collisions.hpp` are header-only — no separate compilation step needed
