#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <vector>
#include "drawing.h"

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
    virtual void draw(Vector2 des) override;
};
class WorldHandler {
    public:
    World background;
    World foreground;
    WorldHandler(const char* backgroundTexture, const char* foregroundTexture, WorldDrawProperty* drawProperty);
    void changeMap (const char* backgroundTexture, const char* foregroundTexture, int width, int height, Vector2 des, std::vector<int>* collisionData);
};

#endif