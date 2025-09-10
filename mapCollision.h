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
    CollisionProperty checkBoundaryCollision(Rectangle characterCollision, Vector2 mapPos, float XOffset, float YOffset);
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
    float updatePropTime{0};
    Prop (const char* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight);
    void drawProp (Vector2 mapPos, float deltaTime);
};
class MapProp: public MapBoundary {
    protected:
    std::vector<Prop> props{};
    public:
    MapProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, int inputCollisionCode);
    void drawAllProps (float scale, Vector2 mapPos, float deltaTime);
};
#endif