#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "../src/classes.hpp"

bool AABBcollision(const Sprite& a, const Sprite& b);
void resolveCollision(Sprite& a, Sprite& b);

#endif