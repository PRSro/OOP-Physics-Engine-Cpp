#pragma once
#include "classes.hpp"
#include <vector>

bool AABBcollision(const Sprite& a, const Sprite& b) {
    return !(a.position.x + a.shape.width()  < b.position.x ||
             b.position.x + b.shape.width()  < a.position.x ||
             a.position.y + a.shape.height() < b.position.y ||
             b.position.y + b.shape.height() < a.position.y);
}

void resolveCollision(Sprite& a, Sprite& b, bool& resolvedX, bool& resolvedY) {
    if (!AABBcollision(a, b)) {
        a.collision = false;
        b.collision = false;
        return;
    }
    a.collision = true;
    b.collision = true;
    double overlapLeft   = (a.position.x + a.shape.width())  - b.position.x;
    double overlapRight  = (b.position.x + b.shape.width())  - a.position.x;
    double overlapTop    = (a.position.y + a.shape.height()) - b.position.y;
    double overlapBottom = (b.position.y + b.shape.height()) - a.position.y;
    double minOverlapX = std::min(overlapLeft,  overlapRight);
    double minOverlapY = std::min(overlapTop,   overlapBottom);
    bool preferX = minOverlapX < minOverlapY;
    if (std::abs(minOverlapX - minOverlapY) < 5.0) {
        double absVx = std::abs(a.velocity_x) + std::abs(b.velocity_x);
        double absVy = std::abs(a.velocity_y) + std::abs(b.velocity_y);
        preferX = absVx >= absVy;
    }
    if (preferX) {
        if (overlapLeft < overlapRight) {
            if      (a.fixed) b.position.x += minOverlapX;
            else if (b.fixed) a.position.x -= minOverlapX;
            else { a.position.x -= minOverlapX * 0.5;
                   b.position.x += minOverlapX * 0.5; }
        } else {
            if      (a.fixed) b.position.x -= minOverlapX;
            else if (b.fixed) a.position.x += minOverlapX;
            else { a.position.x += minOverlapX * 0.5;
                   b.position.x -= minOverlapX * 0.5; }
        }
        if (!resolvedX) {
            if (!a.fixed) a.velocity_x = -a.velocity_x * a.material.restitution;
            if (!b.fixed) b.velocity_x = -b.velocity_x * b.material.restitution;
            resolvedX = true;
        }
    } else {
        if (overlapTop < overlapBottom) {
            if (a.velocity_y > 0) {
                if      (a.fixed) b.position.y += minOverlapY;
                else if (b.fixed) a.position.y -= minOverlapY;
                else { a.position.y -= minOverlapY * 0.5;
                       b.position.y += minOverlapY * 0.5; }
            } else {
                if      (a.fixed) b.position.y -= minOverlapY;
                else if (b.fixed) a.position.y += minOverlapY;
                else { a.position.y += minOverlapY * 0.5;
                       b.position.y -= minOverlapY * 0.5; }
            }
        } else {
            if (b.fixed && a.position.y < b.position.y) {
                a.position.y -= minOverlapY;
            } else {
                if      (a.fixed) b.position.y += minOverlapY;
                else if (b.fixed) a.position.y -= minOverlapY;
            }
        }
        if (!resolvedY) {
            if (!a.fixed) a.velocity_y = -a.velocity_y * a.material.restitution;
            if (!b.fixed) b.velocity_y = -b.velocity_y * b.material.restitution;
            resolvedY = true;
        }
    }
}

void resolveAllCollisions(Sprite& player, std::vector<Sprite*>& obstacles, int maxIter = 5) {
    for (int iter = 0; iter < maxIter; iter++) {
        bool anyCollision = false;
        bool resolvedX = false;
        bool resolvedY = false;
        for (auto* obs : obstacles) {
            if (AABBcollision(player, *obs)) {
                resolveCollision(player, *obs, resolvedX, resolvedY);
                anyCollision = true;
            }
        }
        if (!anyCollision) break;
    }
}
/*
void State_resolution(Sprite& a, Sprite &b){
    std::string collision_type;
    if(a.material.type==b.material.type && a.material.type=="Solid"){
        if(a.material.state==b.material.state && a.material.state=="Elastic"){
            collision_type="Elastic-Elastic";
        }
        else if (a.material.state=="Plastic" && a.material.state==b.material.state){
            collision_type="Plastic-Plastic";
        }
    }
}

* void Physical_Collision(std::string collision_type){

}*/