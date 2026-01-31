#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <raylib.h>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class SwitchDirectionEnum {
    Top = 1,
    Bottom = 2,
    Left = 3,
    Right = 4
};
class ObjectDetail {
    Rectangle dimension;
    std::unordered_map<std::string, json> properties{};

    public:
        ObjectDetail(Rectangle dimension, std::unordered_map<std::string, json> properties);
        Rectangle getDimension();
        json getProperty(std::string propertyKey);
};
struct Location {
    int x;
    int y;
};
struct CollisionProperty {
    bool isCollide;
    Rectangle collider;
};
enum PlayerState {
    Idle,
    Walking,
    Running,
    Jumping,
    Attacking,
    Hurt,
    Dead
};
enum PlayerDirection {
    Right = 0,
    Down = 1,
    Left = 2,
    Up = 3    
};
CollisionProperty checkIsCollide (Rectangle firstCollider, Rectangle secondCollider, float XOffset = 0, float YOffset = 0);
void findPath (std::vector<std::vector<int>>* tileArray, Vector2 actorPos, Vector2* direction, int colorCode, int startTileColorCode);
std::vector<std::vector<int>> arrayTo2DArray (std::vector<int>* arrayData, int mapWidth);
CollisionProperty checkCollisionTile(std::vector<std::vector<int>>* tileArray, Rectangle characterCollision, Vector2 direction, int colorCode);
CollisionProperty checkPlayerCollisionTile(
    std::vector<std::vector<int>>* tileArray, 
    Rectangle characterCollision, 
    Vector2 characterWorldPos, 
    Vector2 direction, 
    int colorCode
);
CollisionProperty checkInteractionTile(
    std::vector<std::vector<int>>* tileArray, 
    Rectangle characterCollision, Vector2 worldPos, 
    float XOffset, 
    float YOffset, 
    int colorCode);
Vector2 Vector2Round(Vector2 inputVector);
std::vector<int> getArrayFromJson(json& jObject, std::string key);
CollisionProperty checkButtonClick(Rectangle buttonRec);
CollisionProperty checkMouseOnHover(Rectangle buttonRec);
CollisionProperty checkCircleInteraction(Vector2 CenterPoint_1, Vector2 CenterPoint_2, float desireDistance);
void drawButtonBackground (Rectangle DestinationRec, Color color, float marginX, float marginY);
CollisionProperty checkAutoSwitchMap (Rectangle playerRec, Rectangle switchRec, SwitchDirectionEnum direction);
std::vector<ObjectDetail> getObjectsFromJsonLayer(json& jObject, std::string layerName, std::vector<std::string> requestedProperties);

#endif