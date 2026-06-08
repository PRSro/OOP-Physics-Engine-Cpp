#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "../src/classes.hpp"

bool AABBcollision(const Sprite& a, const Sprite& b);
void resolveCollision(Sprite& a, Sprite& b,
                      bool& resolvedX, bool& resolvedY,
                      double deltaTime);
void resolveAllCollisions(Sprite& player,
                          std::vector<Sprite*>& obstacles,
                          double deltaTime,
                          int maxIter = 5);

#endif