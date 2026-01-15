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
Shop::Shop(Rectangle shopDimension, std::string name, std::vector<ShopItemProperties> allShopItemProperties): 
    shopDimension(shopDimension),
    name(name) {
        for (ShopItemProperties property: allShopItemProperties) {
            shopItems.emplace_back(property.textureSrc, property.name, property.heal, property.energyHeal);
        }
};
std::vector<ShopItem>* Shop::getShopItems(){
    return &shopItems;
}
std::string Shop::getShopName() {
    return name;
}
Rectangle Shop::getShopDimension(Vector2 mapPos) {
    Rectangle screenShopDimension = {shopDimension.x + mapPos.x, shopDimension.y + mapPos.y, shopDimension.width, shopDimension.height};
    return screenShopDimension;
}
WorldSet::WorldSet(const char* backgroundTexture, const char* foregroundTexture, int mapWidth, int mapHeight, 
    std::vector<int>* collisionData, std::vector<MapProp*>* worldProps, std::string mapPropertyPath, WorldEnums worldName): 
    drawProperty(mapWidth, mapHeight, collisionData), background(backgroundTexture, &drawProperty), foreground(foregroundTexture, &drawProperty), worldProps(worldProps),
    worldName(worldName)  {
        std::fstream file (mapPropertyPath);
    if (!file.is_open()) {
        std::cout<<"error, can't open file";
    } else {
        json j;
        file >> j;
        auto layers = j["layers"];
        std::vector<int>::iterator tempCollision = std::find_if(collisionData->begin(), collisionData->end(), [](int data) {
            return data != 0;
        });
        if (tempCollision != collisionData->end()) {
            collisionCode = *tempCollision;
        }
        auto spawnPointLayer = std::find_if(layers.begin(), layers.end(), [](const json& layer) {
                return layer["name"].get<std::string>() == "entry point";
            });
        if (spawnPointLayer != layers.end()) {
            for (auto &obj : (*spawnPointLayer)["objects"]) {
                int spawnIndex{};
                int spawnToIndex{};
                int width{(int)(obj["width"].get<float>()*MAP_SCALE)};
                int height{(int)(obj["height"].get<float>()*MAP_SCALE)};
                std::string inputSwitchToMap{};
                Vector2 inputLocation = {std::round(obj["x"].get<float>()*MAP_SCALE), std::round(obj["y"].get<float>()*MAP_SCALE)};
                if (obj.contains("properties")) {
                    for (auto &property : obj["properties"]) {
                        if (property["name"].get<std::string>() == "spawn index") {
                            spawnIndex = property["value"].get<int>();
                        }
                        if (property["name"].get<std::string>() == "spawn target") {
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
        auto shopLocationLayer = std::find_if(layers.begin(), layers.end(), [](const json& layer) {
                return layer["name"].get<std::string>() == "shop location";
            });
        if (shopLocationLayer != layers.end()) {
            auto allShop = (*shopLocationLayer)["objects"];
            auto it = allShopItems.find(worldName);
            if (it != allShopItems.end()) {
                AllShopProperties worldShops = it->second;
                for (auto &shop: allShop) {
                    std::string tempShopName{};
                    if (shop.contains("properties")) {
                        for (auto &property: shop["properties"]) {
                            if (property["name"] == "shopName") {
                                tempShopName = property["value"];
                            }
                        }
                    }
                    Rectangle tempDimension = {shop["x"].get<float>()*MAP_SCALE, shop["y"].get<float>()*MAP_SCALE, shop["width"].get<float>()*MAP_SCALE, shop["height"].get<float>()*MAP_SCALE};
                    auto it_2 = worldShops.find(tempShopName);
                    if (it_2 != worldShops.end()) {
                        std::vector<ShopItemProperties> availableItemsProperties = it_2->second;
                        shops.emplace_back(tempDimension, tempShopName, availableItemsProperties);
                    }
                }
            }
        }
    }
}
WorldEnums WorldSet::getWorldName() {
    return worldName;
}
std::vector<Shop>* WorldSet::getCurrentWorldShops() {
    return &shops;
};
std::vector<ShopItem>* WorldSet::getShopItems (std::string name) {
    auto shopToOpen = std::find_if(shops.begin(), shops.end(), [name](Shop shop){
        return shop.getShopName() == name;
    });
    std::cout<< "first shop item is "<< (*(*shopToOpen).getShopItems())[0].name;
    std::vector<ShopItem>* shopItems = (*shopToOpen).getShopItems();
    return shopItems;
};
int WorldSet::getCollisionCode() {
    return collisionCode;
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
        
    } else {
        std::cout<<"can't find spawn point";
    }
    return spawnLocation;
};
void WorldSet::saveAIPlayers(std::vector<AIPlayer> currentAIPlayers) {
    AIPlayers = currentAIPlayers;
};
std::vector<AIPlayer>* WorldSet::getAIPlayers() {
    return &AIPlayers;
}
MapSwitcherProp::MapSwitcherProp (Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height) : location(location), screenPos(location), switchToMap(switchToMap),
spawnIndex(spawnIndex), spawnToIndex(spawnToIndex), width(width), height(height) {
};
void MapSwitcherProp::draw(Vector2 des) {
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
    if (switchToMap == "office_interior_2") spawnToDetail.targetMap = InteriorOffice2;
    if (switchToMap == "hardware_interior") spawnToDetail.targetMap = InteriorHardware;
    if (switchToMap == "sunna_interior") spawnToDetail.targetMap = InteriorSunna;
    return spawnToDetail;
};