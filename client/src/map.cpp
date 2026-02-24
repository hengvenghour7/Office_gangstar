#include "map.h"
#include "globalVar.h"
#include "helpers.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>

using json = nlohmann::json;

WorldDrawProperty::WorldDrawProperty (int width, int height, std::vector<int>* collisionData) : collisionArray(arrayTo2DArray(collisionData, width)) {
};
void WorldDrawProperty::changeProperty(int width, int height, Vector2 des, std::vector<int>* collisionData) {
    
};
Shop::Shop(Rectangle shopDimension, std::string name, std::vector<ShopItemProperties> allShopItemProperties): 
    shopDimension(shopDimension),
    name(name) {
        for (ShopItemProperties property: allShopItemProperties) {
            shopItems.emplace_back(property.textureSrc, property.name, property.heal, property.energyHeal, property.price, property.description);
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
    std::vector<int>* collisionData, std::vector<MapProp*>* worldProps, std::string mapPropertyPath,
    WorldEnums worldName, Player& player, int levelAmount, int AI_amount): 
    drawProperty(mapWidth, mapHeight, collisionData), background(backgroundTexture, &drawProperty), foreground(foregroundTexture, &drawProperty), worldProps(worldProps),
    worldName(worldName)  {
        if (AI_amount > 0) {
            std::random_device rd;

            // Create a Mersenne Twister generator seeded with rd
            std::mt19937 gen(rd());

            std::uniform_real_distribution<float> dis(1.f, 4.0f);
            // Generate a random number
            float randomValue = dis(gen);
            for (int i = 0; i< AI_amount; i++) {
                AIPlayers.emplace_back("resources/image/character/workingman.png", &player, i, dis(gen)/4, dis(gen) + 20, getWorldCollisionArray());
            }
        }
        std::fstream file (mapPropertyPath);
    if (!file.is_open()) {
        std::cout<<"error, can't open file";
    } else {
        json j;
        file >> j;
        auto temp_interactable_items = getObjectsFromJsonLayer(j, "pickable_items", {"imageSrc", "textureWidth", "textureHeight"});
        for (ObjectDetail obj: temp_interactable_items) {
            interactableItemList.emplace_back(
                obj.getProperty("textureWidth").get<int>(),
                obj.getProperty("textureHeight").get<int>(),
                obj.getProperty("imageSrc").get<std::string>().c_str(),
                obj.getDimension());
        }
        auto temp_level_switchers = getObjectsFromJsonLayer(j, "level_switcher", {"option_1", "option_2"});
        for (int i =0 ; i < temp_level_switchers.size(); i++) {
            ObjectDetail obj = temp_level_switchers[i];
            autoLevelSwitcherList.emplace(i, 
                LevelSwitcher(obj.getProperty("option_1").get<int>(), obj.getProperty("option_2").get<int>(), obj.getDimension()));
        }
        if (levelAmount > 0) {
            for (int i = 0; i < levelAmount; i++) {
                std::vector<int> tmpDataArray = getArrayFromJson(j, "collision_lvl_" + std::to_string(i + 1));
                std::vector<std::vector<int>> tmpCollisionArray = arrayTo2DArray( &tmpDataArray, mapWidth);
                std::vector<int>::iterator tmpCollisionCode = std::find_if(tmpDataArray.begin(), tmpDataArray.end(), [](int data) {
                    return data != 0;
                });
                if (tmpCollisionCode != tmpDataArray.end()) {
                    levelDataList[i + 1] = {tmpCollisionArray, *tmpCollisionCode};
                }
            }
            std::cout<< "all level are sssss " << levelDataList.size() <<std::flush;
        }
        auto layers = j["layers"];
        std::vector<int>::iterator tempCollision = std::find_if(collisionData->begin(), collisionData->end(), [](int data) {
            return data != 0;
        });
        if (tempCollision != collisionData->end()) {
            collisionCode = *tempCollision != 0 ? *tempCollision : 1;
        }
        levelDataList[0] = {drawProperty.collisionArray, collisionCode};
        auto spawnPointLayer = std::find_if(layers.begin(), layers.end(), [](const json& layer) {
                return layer["name"].get<std::string>() == "entry point";
            });
        if (spawnPointLayer != layers.end()) {
            for (auto &obj : (*spawnPointLayer)["objects"]) {
                int spawnIndex{};
                int spawnToIndex{};
                bool isAuto{false};
                SwitchDirectionEnum direction = SwitchDirectionEnum::Top;
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
                        if (property["name"] == "isAuto") {
                            isAuto = property["value"].get<bool>();
                        }
                        if (property["name"] == "direction") {
                            std::string temp_direction = property["value"].get<std::string>();
                            if (temp_direction == "top") {
                                direction = SwitchDirectionEnum::Top;
                            }
                            if (temp_direction == "bottom") {
                                direction = SwitchDirectionEnum::Bottom;
                            }
                            if (temp_direction == "left") {
                                direction = SwitchDirectionEnum::Left;
                            }
                            if (temp_direction == "right") {
                                direction = SwitchDirectionEnum::Right;
                            }
                        }
                    }
                }
                if (!isAuto) {
                    mapSwitchersList.emplace(spawnIndex, MapSwitcherProp(inputLocation, inputSwitchToMap, spawnIndex , spawnToIndex, width, height));
                } else {
                    autoMapSwitcherList.emplace(spawnIndex, AutoMapSwitcherProp(inputLocation, inputSwitchToMap, spawnIndex , spawnToIndex, width, height, direction));
                }
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
    for (Drawing &item : interactableItemList) {
        allWorldProps.push_back(&item);
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
        auto it2 = autoMapSwitcherList.find(spawnIndex);
        if (it2 != autoMapSwitcherList.end()) {
            spawnLocation = Vector2Subtract(it2->second.getSpawnLocation(), {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}) ;
        } else {
            std::cout<<"can't find spawn point";
        }
    }
    return spawnLocation;
};
void WorldSet::saveAIPlayers(std::vector<AIPlayer> currentAIPlayers) {
    AIPlayers = currentAIPlayers;
};
std::unordered_map<int , LevelSwitcher>* WorldSet::getAutoLevelSwitcherList() {
    return &autoLevelSwitcherList;
};
std::unordered_map<int , LevelData>* WorldSet::getLevelDataList() {
    return &levelDataList;
};
void WorldSet::handleItemPickUp(Player& player, Vector2 mapPos) {
    if (player.getHoldingItems()->size() > 0) {
        interactableItemList.push_back((*player.getHoldingItems())[0]);
        player.getHoldingItems()->pop_back();
        return;
    }
    for (int i =0; i < interactableItemList.size() ; i++) {
        InteractableItem& item = interactableItemList[i];
        if (checkCircleInteraction(player.getCenter(mapPos), item.getCenter(mapPos), 100).isCollide) {
            player.replaceHoldingItems(item);
            interactableItemList.erase(interactableItemList.begin() + i);
            return;
        }
    }
}
std::vector<AIPlayer>* WorldSet::getAIPlayers() {
    return &AIPlayers;
}
std::vector<InteractableItem>* WorldSet::getInteractableItem() {
    return &interactableItemList;
};
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
    Rectangle collisionBox = {screenPos.x, screenPos.y, (float)width, (float)height};
    return collisionBox;
}
AutoMapSwitcherProp::AutoMapSwitcherProp(Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height, SwitchDirectionEnum direction):
    MapSwitcherProp(location, switchToMap, spawnIndex, spawnToIndex, width, height),
    direction(direction) {
}
LevelSwitcher::LevelSwitcher(int option1, int option2, Rectangle dimension): option1(option1), option2(option2),
    dimension(dimension) {
}
Rectangle LevelSwitcher::getScreenPosDimension(Vector2 mapPos) {
    Rectangle screenPosDimension = {dimension.x + mapPos.x, dimension.y + mapPos.y, dimension.width, dimension.height};
    return screenPosDimension;
}
void LevelSwitcher::draw(Vector2 mapPos) {
    Rectangle screenPosDimension = {dimension.x + mapPos.x, dimension.y + mapPos.y, dimension.width, dimension.height};
    DrawRectangle(screenPosDimension.x, screenPosDimension.y, screenPosDimension.width, screenPosDimension.height, GREEN);
}
SwitchDirectionEnum AutoMapSwitcherProp::getDirection() {
    return direction;
}
void WorldSet::setSwitchersPos (Vector2 mapPos) {
    for (auto &[key, value] : mapSwitchersList) {
        value.setScreenPos(mapPos);
    }
    for (auto &[key, value] : autoMapSwitcherList) {
        value.setScreenPos(mapPos);
    }
}
void WorldSet::addItemtoWorld (InteractableItem item) {

};
std::unordered_map<int , MapSwitcherProp>* WorldSet::getMapSwitchersList(){
    return &mapSwitchersList;
};
std::unordered_map<int, AutoMapSwitcherProp>* WorldSet::getAutoMapSwitcherList() {
    return &autoMapSwitcherList;
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
    if (switchToMap == "upper_map") spawnToDetail.targetMap = WorldEnums::UpperMap;
    return spawnToDetail;
};