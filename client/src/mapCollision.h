#ifndef MAPCOLLISION_H
#define MAPCOLLISION_H

#include <vector>
#include <iostream>
#include <raylib.h>
#include "helpers.h"
#include <raymath.h>
#include "drawing.h"

struct FollowPathDetail {
    bool isFollowPath;
    int pathCode;
};
struct PropDrawCondition {
    int collisionCode;
    Texture2D imagePath;
    int startCol;
    int startRow;
    int maxCols;
    int maxRows;
    float width;
    float height;
    FollowPathDetail followPathDetail{false, 0};
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
class Prop: public Drawing{
    private:
    public:
    Texture2D* propTexture;
    float x;
    float y;
    Vector2 direction{0,0};
    float initialCol{8};
    int startCol{};
    float maxCol{10};
    float propWidth{};
    float propHeight{};
    float row{};
    float updatePropTime{0};
    int locationCode;
    float scale;
    bool isFirstAction{};
    FollowPathDetail followPathDetail;
    const char* interactionSpeech{"Hello there how are you doing ?"};
    std::vector<const char*> interactionSpeechs{"Hello there how are you doing ?", "Hey!!!", "Don't touch me"};
    int speechIndex{0};
    Prop (Texture2D* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputRow, float inputMaxCol, float scale, int locationCode, FollowPathDetail followPathDetail);
    virtual void draw (Vector2 mapPos) override;
    void setIsFirstAction(bool isFirstACtion);
    void updateAnimation(float deltaTime, bool isBackward, bool isPauseisPauseAfterAnimated);
    Vector2 getCenter(Vector2 mapPos);
    const char* getInteractionSpeech ();
    void displayerInteractionText(Vector2* speechLocation, Texture2D* speechBackground);
};
class MapProp: public Drawing {
    protected:
    std::vector<std::vector<int>> locationArray;
    std::vector<Prop> props{};
    std::vector<Prop*> followPathProps{};
    std::vector<PropDrawCondition>* PropDrawConditions;
    Vector2 speechLocation;
    float scale;
    public:
    MapProp (std::vector<int>* inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition>* propCollisionConditions, float scale);
    virtual void draw (Vector2 mapPos) override;
    CollisionProperty checkInteraction (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset);
    void moveProps (float speedX, float speedY);
    void moveProps2 (float speedX, float speedY);
    void updateAnimation(float deltaTime, bool isBackward = false, bool isPauseisPauseAfterAnimated = false);
    std::vector<Prop>* getMapProp();
};
struct Map {
    Texture2D mapTexture;
};
class InteractableProp: public MapProp {
    protected:
    bool isOn{};
    public:
    InteractableProp (std::vector<int>* inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition>* propCollisionConditions, float scale);
    void toggleDraw (float scale, Vector2 mapPos, float deltaTime);
    void toggleIsOn();
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