#ifndef MAPCOLLISION_H
#define MAPCOLLISION_H

#include <vector>
#include <iostream>
#include <raylib.h>
#include "helpers.h"

struct PropDrawCondition {
    int collisionCode;
    const char* imagePath;
    int startCol;
    int startRow;
    int maxCols;
    int maxRows;
    float width;
    float height;
};
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
    void setCollisionData(std::vector<int>* mapCollisionData, int mapWidth, int mapHeight, int collisionCode);
};
class Prop {
    private:
    public:
    Texture2D propTexture{};
    float x;
    float y;
    float initialCol{8};
    int startCol{};
    float maxCol{10};
    float propWidth{};
    float propHeight{};
    float row{};
    float updatePropTime{0};
    float scale;
    Prop (const char* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputRow, float inputMaxCol, float scale);
    void drawProp (Vector2 mapPos, float deltaTime, bool isBackward = false, bool isPauseAfterAnimated = false);
};
class MapProp: public MapBoundary {
    protected:
    std::vector<Prop> props{};
    std::vector<PropDrawCondition> PropDrawConditions;
    float scale;
    public:
    MapProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition> propCollisionConditions, float scale);
    virtual void drawAllProps (float scale, Vector2 mapPos, float deltaTime);
    CollisionProperty checkInteraction (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset);
    void moveProps (float speedX, float speedY);
};
struct Map {
    Texture2D mapTexture;
};
class InteractableProp: public MapProp {
    protected:
    bool isOn{};
    public:
    InteractableProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition> propCollisionConditions, float scale);
    virtual void drawAllProps (float scale, Vector2 mapPos, float deltaTime) override;
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