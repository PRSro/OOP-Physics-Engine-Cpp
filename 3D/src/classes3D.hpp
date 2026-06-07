#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include "../../2D/src/classes.hpp"
#include "../src/shape3D.hpp"

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

class Sprite3D {
public:
    double mass;
    Vec3 velocity;
    Vec3 position;
    Vec3 forces;
    bool fixed;
    Material material;
    Shape shape;
    Shape3D shape3D;

    Sprite3D(double m, Material mat, Vec3 pos, Shape shp, Shape3D shp3D)
        : mass(m), position(pos), material(mat), shape(shp), shape3D(shp3D), fixed(false),
          velocity(0, 0, 0), forces(0, 0, 0) {}

    void applyForce(double fx, double fy, double fz) {
        forces.x += fx;
        forces.y += fy;
        forces.z += fz;
    }

    void update(double deltaTime, double winW, double winH, double winD) {
        if (fixed) return;

        forces.y += mass * 98.0;

        velocity.x += (forces.x / mass) * deltaTime;
        velocity.y += (forces.y / mass) * deltaTime;
        velocity.z += (forces.z / mass) * deltaTime;

        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        position.z += velocity.z * deltaTime;

        forces = Vec3(0, 0, 0);

        if (position.x + shape.width() >= winW) {
            position.x = winW - shape.width();
            velocity.x = -velocity.x * material.restitution;
        }
        if (position.x <= 0.0) {
            position.x = 0.5;
            velocity.x = -velocity.x * material.restitution;
        }

        if (position.y + shape.height() >= winH) {
            position.y = winH - shape.height();
            velocity.y = -velocity.y * material.restitution;
            velocity.x *= (1.0 - material.mu * deltaTime);
        }
        if (position.y <= 0.0) {
            position.y = 0.5;
            velocity.y = -velocity.y * material.restitution;
        }

        if (position.z + shape3D.depth >= winD) {
            position.z = winD - shape3D.depth;
            velocity.z = -velocity.z * material.restitution;
        }
        if (position.z <= 10.0) {
            position.z = 10.0;
            velocity.z = -velocity.z * material.restitution;
        }
    }
};
