#pragma once
#include "classes.hpp"

bool AABBcollision(const Sprite& a, const Sprite& b) {
    return !(a.position.x + a.shape.width()  < b.position.x ||
             b.position.x + b.shape.width()  < a.position.x ||
             a.position.y + a.shape.height() < b.position.y ||
             b.position.y + b.shape.height() < a.position.y);
}

void resolveCollision(Sprite& a, Sprite& b) {
    if (!AABBcollision(a, b)) return;
    a.collision = true;
    b.collision = true;
    if (a.fixed && b.fixed) return;
    if (a.fixed) {
        b.velocity_y=-b.velocity_y*b.material.restitution;
        b.velocity_x=-b.velocity_x*b.material.restitution;
        return;
    }
    if (b.fixed) {
        a.velocity_y=-a.velocity_y*a.material.restitution;
        a.velocity_x=-a.velocity_x*a.material.restitution;
        return;
    }
    double v1=a.velocity_y;
    double v2=b.velocity_y;
    double v3=a.velocity_x;
    double v4=a.velocity_x;
    a.velocity_y=v2*b.mass/a.mass*a.material.restitution;
    b.velocity_y=v1*a.mass/b.mass*b.material.restitution;
    a.velocity_x=v2*b.mass/a.mass*a.material.restitution;
    b.velocity_x=v1*a.mass/b.mass*b.material.restitution;
}