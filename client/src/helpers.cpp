#include "helpers.h"
#include <raylib.h>
#include <vector>
#include "globalVar.h"
#include <raymath.h>

CollisionProperty checkIsCollide (Rectangle firstCollider, Rectangle secondCollider, Vector2 mapPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    if (firstCollider.x + XOffset < secondCollider.x + secondCollider.width && firstCollider.x + firstCollider.width > secondCollider.x && 
        firstCollider.y < secondCollider.y + secondCollider.height && firstCollider.y + firstCollider.height > secondCollider.y) {
            collision1.isCollide = true;
            collision1.collider = secondCollider;
    };
    return collision1;
}
std::vector<std::vector<int>> arrayTo2DArray (std::vector<int> arrayData, int mapWidth) {
    std::vector<std::vector<int>> array2D {};
    for (int j=0 ; j < (int)arrayData.size(); j+= mapWidth) {
        std::vector<int> chunk{};
        for (int i = j; i< j + mapWidth; i++) {
            chunk.push_back(arrayData[i]);
        }
        array2D.push_back(chunk);
    }
    return array2D;
}
void findPath (std::vector<std::vector<int>>* tileArray, Vector2 actorPos, Vector2* direction, int colorCode, int startTileColorCode) {
    int tileX = (int)((actorPos.x)/TILE_SIZE/MAP_SCALE); // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)((actorPos.y)/TILE_SIZE/MAP_SCALE);
    Vector2 nextDirection{0,0};
    if (direction->x >= 0 && tileX >= 0 && tileY >= 0) {
        if ((*tileArray)[tileY][tileX + 1] == colorCode) {
            direction->x = 1;
            direction->y = 0;
            return;
        }
        if (direction->y <= 0) {
            if ((*tileArray)[tileY-1][tileX] == colorCode) {
                direction->y = -1;
                direction->x = 0;
                return;
            }
            if ((*tileArray)[tileY + 2][tileX] == colorCode) {
                direction->y = 1;
                direction->x = 0;
                return;
            }
            direction->x = -1;
            direction->y = 0;
        }
        if (direction->y > 0) {
            if ((*tileArray)[tileY+1][tileX] == colorCode) {
                direction->y = 1;
                direction->x = 0;
                return;
            }
            if ((*tileArray)[tileY][tileX - 1] == colorCode) {
            direction->x = -1;
            direction->y = 0;
            return;
            }
            if ((*tileArray)[tileY-1][tileX] == colorCode) {
                direction->y = -1;
                direction->x = 0;
                return;
            }
        }
        
    }
    if (direction->x < 0 && tileX >= 0 && tileY >= 0) {
        if ((*tileArray)[tileY][tileX - 1] == colorCode && tileX - 1 >= 0) {
            direction->x = -1;
            direction->y = 0;
            return;
        }
        direction->x = 1;
        direction->y = 0;
    }
};
CollisionProperty checkCollisionTile(std::vector<std::vector<int>>* tileArray, Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset, int colorCode) {
    CollisionProperty collision {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y + 8};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, worldPos);
    Vector2 currentTile {(int)((playerWorldPos.x)/TILE_SIZE/MAP_SCALE), (int)((playerWorldPos.y)/TILE_SIZE/MAP_SCALE)};
    Vector2 frontTile {(int)((playerWorldPos.x + XOffset)/TILE_SIZE/MAP_SCALE), (int)((playerWorldPos.y + YOffset)/TILE_SIZE/MAP_SCALE)};
    Vector2 topRightTile {currentTile.x + 1, currentTile.y -1};
    Vector2 topLeftTile {currentTile.x - 1, currentTile.y -1};
    Vector2 BottomLeftTile {currentTile.x -1 , currentTile.y + 1};
    Vector2 BottomRightTile {currentTile.x + 1, currentTile.y + 1};

    if ( (frontTile.y >= 0 && frontTile.y < tileArray->size() && (*tileArray)[frontTile.y][frontTile.x] == colorCode) ||
        (topRightTile.y >= 0 && topRightTile.y < tileArray->size() && (*tileArray)[topRightTile.y][topRightTile.x])  ||
        (topLeftTile.y >= 0 && topLeftTile.y < tileArray->size() && (*tileArray)[topLeftTile.y][topLeftTile.x])  ||
        (BottomLeftTile.y >= 0 && BottomLeftTile.y < tileArray->size() && (*tileArray)[BottomLeftTile.y][BottomLeftTile.x])  ||
        (BottomRightTile.y >= 0 && BottomRightTile.y < tileArray->size() && (*tileArray)[BottomRightTile.y][BottomRightTile.x])) {
            collision.isCollide = true;
            return collision;
        }
    return collision;
}
CollisionProperty checkInteractionTile(std::vector<std::vector<int>>* tileArray, Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset, int colorCode) {
    CollisionProperty collision1 {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y + 8};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, worldPos);
    int tileX = (int)(playerWorldPos.x + XOffset)/16/1.5; // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)(playerWorldPos.y + YOffset)/16/1.5;
    if ((*tileArray)[tileY][tileX] == colorCode) {
            collision1.isCollide = true;
            return collision1;
        }
    return collision1;
};
Vector2 Vector2Round(Vector2 inputVector) {
    Vector2 newVector2 = inputVector;
    newVector2.x = (int)(std::round(newVector2.x));
    newVector2.y = (int)(std::round(newVector2.y));
    return newVector2;
};