#include <raylib.h>
#include "helpers.h"

CollisionProperty checkIsCollide (Rectangle firstCollider, Rectangle secondCollider, Vector2 mapPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    if (firstCollider.x + XOffset < secondCollider.x + secondCollider.width && firstCollider.x + firstCollider.width > secondCollider.x && firstCollider.y < secondCollider.y + secondCollider.height && firstCollider.y + firstCollider.height > secondCollider.y) {
            collision1.isCollide = true;
    };
    return collision1;
}