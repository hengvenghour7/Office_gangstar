#ifndef MAPCOLLISION_H
#define MAPCOLLISION_H

#include <vector>
#include <iostream>
#include <raylib.h>
#include "helpers.h"


class MapBoundary {
    protected:
    std::vector<std::vector<int>> dataArray {};
    int mapWidth;
    int mapHeight;
    int collisionCode{};
    public: 
    MapBoundary (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, int inputCollisionCode);
    int getWidth ();
    std::vector<std::vector<int>> getCollisionBoundary();
    void drawBoundary(float scale, Vector2 mapPos);
    CollisionProperty checkInteractionBoundary (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset, int colorCode);
    CollisionProperty checkBoundaryCollision(Rectangle characterCollision, Vector2 mapPos, float XOffset, float YOffset);
    void setCollisionData(std::vector<int>* mapCollisionData, int mapWidth, int mapHeight);
};
class Prop {
    private:
    public:
    Texture2D propTexture{};
    float x;
    float y;
    float initialCol{8};
    float maxCol{10};
    float propWidth{};
    float propHeight{};
    float row{};
    float updatePropTime{0};
    Prop (const char* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputMaxCol, float inputRow);
    void drawProp (Vector2 mapPos, float deltaTime);
};
class MapProp: public MapBoundary {
    protected:
    std::vector<Prop> props{};
    public:
    MapProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, int inputCollisionCode);
    void drawAllProps (float scale, Vector2 mapPos, float deltaTime);
    CollisionProperty checkInteraction (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset);
};
struct Map {
    Texture2D mapTexture;
};
class MapHandler {
    private:
        Texture2D outsideMap{LoadTexture("resources/image/office_gang_map.png")};
        Texture2D shopInteriorMap{LoadTexture("resources/image/shop_interior.png")};
        Texture2D drawTexture{};
        Vector2* mapPos{};
        std::vector<int>* mapCollisionData;
        float scale;
    public:
        MapHandler(Vector2* inputMapPos, float inputScale, std::vector<int>* mapCollisionData);
        void drawMap(Vector2 mapPos);
        void changeMap (Map inputMap);
};
#endif