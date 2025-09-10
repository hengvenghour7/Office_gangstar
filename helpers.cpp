#include <raylib.h>
#include "helpers.h"

CollisionProperty checkIsCollide (Rectangle firstCollider, Rectangle secondCollider, Vector2 mapPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    if (firstCollider.x + XOffset + firstCollider.width > secondCollider.x && firstCollider.x + XOffset < secondCollider.x + 16 && 
        firstCollider.y + YOffset + firstCollider.height > secondCollider.y && firstCollider.y + YOffset < secondCollider.y + 16) {
            collision1.isCollide = true;
    };
    return collision1;
}