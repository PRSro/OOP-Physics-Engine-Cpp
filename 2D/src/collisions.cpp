#pragma once
#include "classes.hpp"
#include <vector>

static auto getRadius = [](const Sprite& s) {
    if (s.shape.type == Shapetype::Circle)
        return s.shape.width() / 2.0;
    return std::sqrt(s.shape.width()*s.shape.width()+s.shape.height()*s.shape.height())/2.0;
};

bool AABBcollision(const Sprite& a, const Sprite& b) {
    double ra = getRadius(a);
    double rb = getRadius(b);
    double ax = a.position.x;
    double ay = a.position.y;
    double bx = b.position.x;
    double by = b.position.y;
    double dist = std::sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by));
    return dist < ra + rb;
}

void resolveCollision(Sprite& a, Sprite& b, bool& resolvedX, bool& resolvedY, double deltaTime) {
    if (!AABBcollision(a, b)) {
        a.collision = false;
        b.collision = false;
        return;
    }
    if (b.fixed || a.fixed) {
    Sprite& moving  = a.fixed ? b : a;
    Sprite& fixed   = a.fixed ? a : b;
    double extX, extY;
if (moving.shape.type == Shapetype::Circle) {
    extX = moving.shape.width() / 2.0;
    extY = extX;
} else if (moving.shape.type == Shapetype::Triangle) {
    extX = moving.shape.width()  / 2.0;
    extY = moving.shape.height() / 3.0;
} else {
    extX = moving.shape.width()  / 2.0;
    extY = moving.shape.height() / 2.0;
}
    double mx = moving.position.x;
    double my = moving.position.y;
    double fx = fixed.position.x;
    double fy = fixed.position.y;
    double hw = fixed.shape.width()  / 2.0;
    double hh = fixed.shape.height() / 2.0;
    double dx = mx - fx;
    double dy = my - fy;
    double overlapX = hw + extX - std::abs(dx);
double overlapY = hh + extY - std::abs(dy);
if (overlapX <= 0 || overlapY <= 0) return;

double effectiveOverlapX = overlapX;
double effectiveOverlapY = overlapY;
if (moving.shape.type == Shapetype::Triangle ||
    moving.shape.type == Shapetype::Rectangle) {
    double absVx = std::abs(moving.velocity_x);
    double absVy = std::abs(moving.velocity_y);
    if (absVy > absVx * 0.5) {
        effectiveOverlapY *= 0.7;
    } else {
        effectiveOverlapX *= 0.7;
    }
}

if (effectiveOverlapX < effectiveOverlapY) {
    double sign = (dx > 0) ? 1.0 : -1.0;
    moving.position.x += sign * overlapX;
    moving.velocity_x = -moving.velocity_x * moving.material.restitution;
    moving.velocity_y *= (1.0 - moving.material.mu * deltaTime * 0.5);
    double contactY = (dy > 0) ? extY : -extY;
    moving.torque += contactY * (-moving.velocity_x)
                     * moving.material.mu * moving.mass * 0.1;
    moving.torque = 0.0;
    moving.angularVel *= (1.0 - moving.material.mu * deltaTime * 60.0);
} else {
    double sign = (dy > 0) ? 1.0 : -1.0;
    moving.position.y += sign * overlapY;
    moving.velocity_y = -moving.velocity_y * moving.material.restitution;
    moving.velocity_y *= (1.0 - moving.material.mu * deltaTime);
    moving.velocity_x *= (1.0 - moving.material.mu * deltaTime);
    double contactX = (dx > 0) ? extX : -extX;
    moving.torque += contactX * moving.velocity_x
                     * moving.material.mu * moving.mass * 0.1;
    double normalForce = moving.mass * moving.forces.gravity;
    moving.angularVel *= (1.0 - moving.material.mu
                          * std::abs(normalForce) * deltaTime * 0.1);
    if (std::abs(moving.angularVel) < 0.05) moving.angularVel = 0.0;
    if (std::abs(moving.torque)     < 0.01) moving.torque     = 0.0;
    moving.collision = (dy > 0);
}
moving.collision = true;
fixed.collision  = true;
return;
    }
    a.collision = true;
    b.collision = true;
    double ra = getRadius(a);
    double rb = getRadius(b);
    double ax = a.position.x;
    double ay = a.position.y;
    double bx = b.position.x;
    double by = b.position.y;
    double dx = ax - bx;
    double dy = ay - by;
    double dist = std::sqrt(dx*dx + dy*dy);
    if (dist == 0) dist = 0.001;
    double overlap = (ra + rb) - dist;
    if (overlap <= 0) return;
    double nx = dx / dist;
    double ny = dy / dist;

    if (a.fixed)       { b.position.x -= nx * overlap; b.position.y -= ny * overlap; }
    else if (b.fixed)  { a.position.x += nx * overlap; a.position.y += ny * overlap; }
    else {
        a.position.x += nx * overlap * 0.5;
        a.position.y += ny * overlap * 0.5;
        b.position.x -= nx * overlap * 0.5;
        b.position.y -= ny * overlap * 0.5;
    }
    if (!resolvedX) {
        if (!a.fixed) a.velocity_x = (a.velocity_x - 2*nx*(a.velocity_x*nx + a.velocity_y*ny)*nx) * a.material.restitution;
        if (!b.fixed) b.velocity_x = (b.velocity_x - 2*nx*(b.velocity_x*nx + b.velocity_y*ny)*nx) * b.material.restitution;
        resolvedX = true;
    }
    if (!resolvedY) {
        if (!a.fixed) a.velocity_y = (a.velocity_y - 2*ny*(a.velocity_x*nx + a.velocity_y*ny)*ny) * a.material.restitution;
        if (!b.fixed) b.velocity_y = (b.velocity_y - 2*ny*(b.velocity_x*nx + b.velocity_y*ny)*ny) * b.material.restitution;
        resolvedY = true;
    }
}

void resolveAllCollisions(Sprite& player,
                          std::vector<Sprite*>& obstacles,
                          double deltaTime,
                          int maxIter = 5) {
    for (int iter = 0; iter < maxIter; iter++) {
        bool anyCollision = false;
        bool resolvedX = false, resolvedY = false;
        for (auto* obs : obstacles) {
            if (AABBcollision(player, *obs)) {
                resolveCollision(player, *obs,
                                 resolvedX, resolvedY, deltaTime);
                anyCollision = true;
            }
        }
        if (!anyCollision) break;
    }
}