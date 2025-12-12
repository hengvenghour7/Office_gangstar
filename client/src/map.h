#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <vector>
#include "drawing.h"
#include "mapCollision.h"

struct WorldProp {

};
struct WorldDrawProperty {
    int width;
    int height;
    std::vector<std::vector<int>> collisionArray;

    WorldDrawProperty(int width, int height, std::vector<int>* collisionData);
    void changeProperty(int width, int height, Vector2 des, std::vector<int>* collisionData);
};
class World : public Drawing {
    WorldDrawProperty* drawProperty;
    Texture2D worldTexture;

    public:
    World(const char* inputTexture, WorldDrawProperty* drawProperty);
    void changeProperty(int width, int height, Vector2 des, std::vector<int>* collisionData);
    std::vector<std::vector<int>>* getWorldCollisionArray ();
    virtual void draw(Vector2 des) override;
};
class WorldSet {
    public:
    WorldDrawProperty drawProperty;
    World background;
    World foreground;
    std::vector<MapProp*>* worldProps;
    WorldSet(const char* backgroundTexture, const char* foregroundTexture, int mapWidth, int mapHeight, std::vector<int>* collisionData, std::vector<MapProp*>* worldProps);
    void changeMap (const char* backgroundTexture, const char* foregroundTexture, int width, int height, Vector2 des, std::vector<int>* collisionData);
    std::vector<std::vector<int>>* getWorldCollisionArray ();
    std::vector<Drawing*> getAllDrawableProps ();
    void animateWorldProps (float deltaTime);
};

#endif