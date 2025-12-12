#include "map.h"
#include "globalVar.h"
#include "helpers.h"

WorldDrawProperty::WorldDrawProperty (int width, int height, std::vector<int>* collisionData) : collisionArray(arrayTo2DArray(collisionData, 150)) {
};
void WorldDrawProperty::changeProperty(int width, int height, Vector2 des, std::vector<int>* collisionData) {
    
};
WorldSet::WorldSet(const char* backgroundTexture, const char* foregroundTexture, int mapWidth, int mapHeight, std::vector<int>* collisionData, std::vector<MapProp*>* worldProps): 
    drawProperty(mapWidth, mapHeight, collisionData), background(backgroundTexture, &drawProperty), foreground(foregroundTexture, &drawProperty), worldProps(worldProps)  {

}
void WorldSet::changeMap (const char* backgroundTexture, const char* foregroundTexture, int width, int height,
    Vector2 des, std::vector<int>* collisionData) {
        background.changeProperty( width, height, des, collisionData);
}

World::World(const char* inputTexture, WorldDrawProperty* drawProperty): worldTexture(LoadTexture(inputTexture)), drawProperty(drawProperty) {

}
void World::changeProperty (int width, int height, Vector2 des, std::vector<int>* collisionData) {
    drawProperty->changeProperty(width, height, des, collisionData);
}
std::vector<std::vector<int>>* WorldSet::getWorldCollisionArray () {
    return background.getWorldCollisionArray();
}
std::vector<std::vector<int>>* World::getWorldCollisionArray() {
    return &drawProperty->collisionArray;
}
void World::draw(Vector2 des) {
    DrawTextureEx(worldTexture, des, 0,MAP_SCALE,WHITE);
}
std::vector<Drawing*> WorldSet::getAllDrawableProps () {
    std::vector<Drawing*> allWorldProps{};
    for (Drawing* propSet : *worldProps) {
        allWorldProps.push_back(propSet);
    }
    return allWorldProps;
};
void WorldSet::animateWorldProps(float deltaTime) {
    for (MapProp* propSet : *worldProps) {
        propSet->updateAnimation(deltaTime);
    }
}