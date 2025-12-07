#include "map.h"
#include "globalVar.h"
#include "helpers.h"

WorldDrawProperty::WorldDrawProperty (int width, int height, std::vector<int>* collisionData) : collisionArray(arrayTo2DArray(collisionData, 150)) {
};
void WorldDrawProperty::changeProperty(int width, int height, Vector2 des, std::vector<int>* collisionData) {
    
};
WorldHandler::WorldHandler (const char* backgroundTexture, const char* foregroundTexture, WorldDrawProperty* drawProperty): 
    background(backgroundTexture, drawProperty), foreground(foregroundTexture, drawProperty) {

}
void WorldHandler::changeMap (const char* backgroundTexture, const char* foregroundTexture, int width, int height,
    Vector2 des, std::vector<int>* collisionData) {
        background.changeProperty( width, height, des, collisionData);
}

World::World(const char* inputTexture, WorldDrawProperty* drawProperty): worldTexture(LoadTexture(inputTexture)), drawProperty(drawProperty) {

}
void World::changeProperty (int width, int height, Vector2 des, std::vector<int>* collisionData) {
    drawProperty->changeProperty(width, height, des, collisionData);
}
std::vector<std::vector<int>>* WorldHandler::getWorldCollisionArray () {
    return background.getWorldCollisionArray();
}
std::vector<std::vector<int>>* World::getWorldCollisionArray() {
    return &drawProperty->collisionArray;
}
void World::draw(Vector2 des) {
    DrawTextureEx(worldTexture, des, 0,MAP_SCALE,WHITE);
}