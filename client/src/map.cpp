#include "map.h"
#include "globalVar.h"
#include "helpers.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

WorldDrawProperty::WorldDrawProperty (int width, int height, std::vector<int>* collisionData) : collisionArray(arrayTo2DArray(collisionData, width)) {
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
MapSwitcherProp::MapSwitcherProp (Vector2 location, std::string switchToMap, int spawnIndex) : location(location), screenPos(location), switchToMap(switchToMap),
spawnIndex(spawnIndex) {
    
};
void MapSwitcherProp::setScreenPos (Vector2 mapPos) {
    screenPos = Vector2Add(location, mapPos);
}
WorldSwitchers::WorldSwitchers () {
    std::fstream file ("resources/maps/office_gang_map.tmj");
    if (!file.is_open()) {
        std::cout<<"error, can't open file";
    } else {
        json j;
        file >> j;
        auto layers = j["layers"];
        auto spawnPointLayer = std::find_if(layers.begin(), layers.end(), [](const json& layer) {
                return layer["name"].get<std::string>() == "entry point";
            });
        if (spawnPointLayer != layers.end()) {
            for (auto &obj : (*spawnPointLayer)["objects"]) {
                int inputSpawnPoint{};
                std::string inputSwitchToMap{};
                Vector2 inputLocation = {std::round(obj["x"].get<float>()*MAP_SCALE), std::round(obj["y"].get<float>()*MAP_SCALE)};
                if (obj.contains("properties")) {
                    for (auto &property : obj["properties"]) {
                        if (property["name"].get<std::string>() == "spawn point") {
                            inputSpawnPoint = property["value"].get<int>();
                        }
                        if (property["name"] == "switchToMap") {
                            inputSwitchToMap = property["value"].get<std::string>();
                        }
                    }
                }
                switchers.emplace_back(inputLocation, inputSwitchToMap, inputSpawnPoint);
            }
        }
    }
}
std::vector<MapSwitcherProp>* WorldSwitchers::getSwitchers () {
    return &switchers;
}
void WorldSwitchers::drawAllSwitchers () {
    for (auto &prop : switchers) {
        Rectangle propRect = prop.getCollision();
        DrawRectangle(propRect.x, propRect.y, propRect.width, propRect.height, PURPLE);
    }
}
void WorldSwitchers::setSwitchersPos (Vector2 mapPos) {
    for (auto &prop : switchers) {
        prop.setScreenPos(mapPos);
    }
}
Rectangle MapSwitcherProp::getCollision() {
    Rectangle collisionBox = {screenPos.x, screenPos.y, 16,16};
    return collisionBox;
}
WorldEnums MapSwitcherProp::getSwitchDestination() {
    WorldEnums newMap{};
    if (switchToMap == "office_interior") newMap = InteriorOffice1;
    if (switchToMap == "supermarket_map") newMap = InteriorSuperMarket;
    return newMap;
};