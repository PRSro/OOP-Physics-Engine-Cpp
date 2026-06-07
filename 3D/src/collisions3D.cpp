#include "classes3D.hpp"
#include "../include/collisions3D.h"
#include <vector>
#include <algorithm>
#include <cmath>

bool AABBcollision3D(const Sprite3D& a, const Sprite3D& b) {
    return !(a.position.x + a.shape.width()  < b.position.x ||
             b.position.x + b.shape.width()  < a.position.x ||
             a.position.y + a.shape.height() < b.position.y ||
             b.position.y + b.shape.height() < a.position.y ||
             a.position.z + a.shape.width()  < b.position.z ||
             b.position.z + b.shape.width()  < a.position.z);
}

void resolveCollision3D(Sprite3D& a, Sprite3D& b) {
    if (!AABBcollision3D(a, b)) return;

    double overlapLeft   = (a.position.x + a.shape.width())  - b.position.x;
    double overlapRight  = (b.position.x + b.shape.width())  - a.position.x;
    double overlapTop    = (a.position.y + a.shape.height()) - b.position.y;
    double overlapBottom = (b.position.y + b.shape.height()) - a.position.y;
    double overlapFront  = (a.position.z + a.shape.width())  - b.position.z;
    double overlapBack   = (b.position.z + b.shape.width())  - a.position.z;

    double minOverlapX = std::min(overlapLeft,  overlapRight);
    double minOverlapY = std::min(overlapTop,   overlapBottom);
    double minOverlapZ = std::min(overlapFront, overlapBack);

    int axis = 0; // 0=x, 1=y, 2=z
    double minVal = minOverlapX;
    if (minOverlapY < minVal) { minVal = minOverlapY; axis = 1; }
    if (minOverlapZ < minVal) { minVal = minOverlapZ; axis = 2; }

    if (std::abs(minOverlapX - minOverlapY) < 5.0 &&
        std::abs(minOverlapX - minOverlapZ) < 5.0) {
        double absVx = std::abs(a.velocity.x) + std::abs(b.velocity.x);
        double absVy = std::abs(a.velocity.y) + std::abs(b.velocity.y);
        double absVz = std::abs(a.velocity.z) + std::abs(b.velocity.z);
        if (absVx >= absVy && absVx >= absVz) axis = 0;
        else if (absVy >= absVz) axis = 1;
        else axis = 2;
    }

    if (axis == 0) {
        double overlap = minOverlapX;
        if (overlapLeft < overlapRight) {
            if      (a.fixed) b.position.x += overlap;
            else if (b.fixed) a.position.x -= overlap;
            else { a.position.x -= overlap * 0.5; b.position.x += overlap * 0.5; }
        } else {
            if      (a.fixed) b.position.x -= overlap;
            else if (b.fixed) a.position.x += overlap;
            else { a.position.x += overlap * 0.5; b.position.x -= overlap * 0.5; }
        }
        if (!a.fixed) a.velocity.x = -a.velocity.x * a.material.restitution;
        if (!b.fixed) b.velocity.x = -b.velocity.x * b.material.restitution;
    } else if (axis == 1) {
        double overlap = minOverlapY;
        if (overlapTop < overlapBottom) {
            if      (a.fixed) b.position.y += overlap;
            else if (b.fixed) a.position.y -= overlap;
            else { a.position.y -= overlap * 0.5; b.position.y += overlap * 0.5; }
        } else {
            if      (a.fixed) b.position.y -= overlap;
            else if (b.fixed) a.position.y += overlap;
            else { a.position.y += overlap * 0.5; b.position.y -= overlap * 0.5; }
        }
        if (!a.fixed) a.velocity.y = -a.velocity.y * a.material.restitution;
        if (!b.fixed) b.velocity.y = -b.velocity.y * b.material.restitution;
    } else {
        double overlap = minOverlapZ;
        if (overlapFront < overlapBack) {
            if      (a.fixed) b.position.z += overlap;
            else if (b.fixed) a.position.z -= overlap;
            else { a.position.z -= overlap * 0.5; b.position.z += overlap * 0.5; }
        } else {
            if      (a.fixed) b.position.z -= overlap;
            else if (b.fixed) a.position.z += overlap;
            else { a.position.z += overlap * 0.5; b.position.z -= overlap * 0.5; }
        }
        if (!a.fixed) a.velocity.z = -a.velocity.z * a.material.restitution;
        if (!b.fixed) b.velocity.z = -b.velocity.z * b.material.restitution;
    }
}

void resolveAllCollisions3D(Sprite3D& player, std::vector<Sprite3D*>& obstacles, int maxIter) {
    for (int iter = 0; iter < maxIter; iter++) {
        bool anyCollision = false;
        for (auto* obs : obstacles) {
            if (AABBcollision3D(player, *obs)) {
                resolveCollision3D(player, *obs);
                anyCollision = true;
            }
        }
        if (!anyCollision) break;
    }
}
