#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <raylib.h>

struct CollisionProperty {
    bool isCollide;
    Rectangle collider;
};
CollisionProperty checkIsCollide (Rectangle firstCollider, Rectangle secondCollider, Vector2 mapPos, float XOffset, float YOffset);

#endif