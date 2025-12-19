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
WorldSet::WorldSet(const char* backgroundTexture, const char* foregroundTexture, int mapWidth, int mapHeight, std::vector<int>* collisionData, std::vector<MapProp*>* worldProps, std::string mapPropertyPath): 
    drawProperty(mapWidth, mapHeight, collisionData), background(backgroundTexture, &drawProperty), foreground(foregroundTexture, &drawProperty), worldProps(worldProps)  {
        std::fstream file (mapPropertyPath);
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
                int spawnIndex{};
                int spawnToIndex{};
                int width{obj["width"].get<int>()*MAP_SCALE};
                int height{obj["height"].get<int>()*MAP_SCALE};
                std::string inputSwitchToMap{};
                Vector2 inputLocation = {std::round(obj["x"].get<float>()*MAP_SCALE), std::round(obj["y"].get<float>()*MAP_SCALE)};
                if (obj.contains("properties")) {
                    for (auto &property : obj["properties"]) {
                        if (property["name"].get<std::string>() == "spawn index") {
                            spawnIndex = property["value"].get<int>();
                        }
                        if (property["name"].get<std::string>() == "spawn point") {
                            spawnToIndex = property["value"].get<int>();
                        }
                        if (property["name"] == "switchToMap") {
                            inputSwitchToMap = property["value"].get<std::string>();
                        }
                    }
                }
                mapSwitchersList.emplace(spawnIndex, MapSwitcherProp(inputLocation, inputSwitchToMap, spawnIndex , spawnToIndex, width, height));
            }
        }
    }
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
Vector2 WorldSet::getSpawnLocation(int spawnIndex) {
    Vector2 spawnLocation{100, 100};
    auto it = mapSwitchersList.find(spawnIndex);
    if (it != mapSwitchersList.end()) {
        // key exists
        spawnLocation = Vector2Subtract(it->second.getSpawnLocation(), {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}) ;
        // spawnLocation = {0 - SCREEN_WIDTH/2, 0 - SCREEN_HEIGHT/2};
        
        std::cout<< "spawn location x "<< spawnLocation.x << " y " << spawnLocation.y;
    }
    return spawnLocation;
};
MapSwitcherProp::MapSwitcherProp (Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height) : location(location), screenPos(location), switchToMap(switchToMap),
spawnIndex(spawnIndex), spawnToIndex(spawnToIndex), width(width), height(height) {
    std::cout<<" location x "<< location.x << " y " << location.y;
};
void MapSwitcherProp::draw(Vector2 des) {
    // std::cout<<"ddddd "<< width;
    DrawRectangle(location.x + des.x, location.y + des.y, width, height, GREEN);
}
void MapSwitcherProp::setScreenPos (Vector2 mapPos) {
    screenPos = Vector2Add(location, mapPos);
}
Vector2 MapSwitcherProp::getSpawnLocation () {
    return location;
}
Rectangle MapSwitcherProp::getCollision() {
    Rectangle collisionBox = {screenPos.x, screenPos.y, width * MAP_SCALE, height * MAP_SCALE};
    return collisionBox;
}
void WorldSet::setSwitchersPos (Vector2 mapPos) {
    for (auto &[key, value] : mapSwitchersList) {
        value.setScreenPos(mapPos);
    }
}
std::unordered_map<int , MapSwitcherProp>* WorldSet::getMapSwitchersList(){
    return &mapSwitchersList;
};
SpawnToDetail MapSwitcherProp::getSwitchDestination() {
    SpawnToDetail spawnToDetail{};
    spawnToDetail.targetSpawnPoint = spawnToIndex;
    if (switchToMap == "office_gang_map") spawnToDetail.targetMap = CenterWorld;
    if (switchToMap == "office_interior") spawnToDetail.targetMap = InteriorOffice1;
    if (switchToMap == "supermarket_map") spawnToDetail.targetMap = InteriorSuperMarket;
    return spawnToDetail;
};