#include <raylib.h>
#include "helpers.h"

CollisionProperty checkIsCollide (Rectangle firstCollider, Rectangle secondCollider, Vector2 mapPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    float secondColliderLocationX = secondCollider.x*16*1.5 + mapPos.x;
    float secondColliderLocationY = secondCollider.y*16*1.5 + mapPos.y;
    if (firstCollider.x + XOffset + firstCollider.width > secondColliderLocationX && firstCollider.x + XOffset < secondColliderLocationX + 16 && 
        firstCollider.y + YOffset + firstCollider.height > secondColliderLocationY && firstCollider.y + YOffset < secondColliderLocationY + 16) {
        collision1.isCollide = true;
    };
    return collision1;
}