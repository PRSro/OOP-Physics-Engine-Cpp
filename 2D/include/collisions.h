#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "../src/classes.hpp"

bool AABBcollision(const Sprite& a, const Sprite& b);
void resolveCollision(Sprite& a, Sprite& b);
void resolveAllCollisions(Sprite& player, std::vector<Sprite*>& obstacles, int maxIter);

#endif