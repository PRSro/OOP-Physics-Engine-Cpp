#ifndef COLLISIONS3D_H
#define COLLISIONS3D_H
#include "../src/classes3D.hpp"

bool AABBcollision3D(const Sprite3D& a, const Sprite3D& b);
void resolveCollision3D(Sprite3D& a, Sprite3D& b);
void resolveAllCollisions3D(Sprite3D& player, std::vector<Sprite3D*>& obstacles, int maxIter);

#endif
