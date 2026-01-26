#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <vector>
#include "drawing.h"
#include "mapCollision.h"
#include "worldEnums.h"
#include "character.h"
#include <unordered_map>
#include "shop/shopData.h"
#include "UIHandler/UI.h"

struct SpawnToDetail {
    WorldEnums targetMap;
    int targetSpawnPoint;
};
class Shop {
    Rectangle shopDimension;
    std::string name;
    std::vector<ShopItem> shopItems{};
    public:
    Shop(Rectangle shopDimension, std::string name, std::vector<ShopItemProperties> allShopItemProperties);
    std::vector<ShopItem>* getShopItems();
    std::string getShopName();
    Rectangle getShopDimension(Vector2 mapPos);
};
class MapSwitcherProp: public Drawing {
    Vector2 location;
    Vector2 screenPos;
    int width;
    int height;
    std::string switchToMap{};
    int spawnIndex;
    int spawnToIndex;
    public:
        MapSwitcherProp(Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height);
        SpawnToDetail getSwitchDestination();
        Rectangle getCollision ();
        virtual void draw(Vector2 ) override;
        void setScreenPos(Vector2 des);
        Vector2 getSpawnLocation();
};
class AutoMapSwitcherProp: public MapSwitcherProp {
        SwitchDirectionEnum direction;
    public:
        AutoMapSwitcherProp(Vector2 location, std::string switchToMap, int spawnIndex, int spawnToIndex, int width, int height, SwitchDirectionEnum direction);
        SwitchDirectionEnum getDirection();
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
    std::string worldPropertySrc;
    std::unordered_map<int , MapSwitcherProp> mapSwitchersList{};
    std::unordered_map<int , AutoMapSwitcherProp> autoMapSwitcherList{};
    std::vector<AIPlayer> AIPlayers;
    int collisionCode{1};
    WorldEnums worldName;
    std::vector<Shop> shops;

    public:
        WorldSet(const char* backgroundTexture, const char* foregroundTexture, int mapWidth, int mapHeight, 
            std::vector<int>* collisionData, std::vector<MapProp*>* worldProps, std::string mapPropertyPath, WorldEnums worldName);
        WorldDrawProperty drawProperty;
        World background;
        World foreground;
        std::vector<MapProp*>* worldProps;
        void changeMap (const char* backgroundTexture, const char* foregroundTexture, int width, int height, Vector2 des, std::vector<int>* collisionData);
        std::vector<std::vector<int>>* getWorldCollisionArray ();
        std::vector<Drawing*> getAllDrawableProps ();
        void animateWorldProps (float deltaTime);
        void setSwitchersPos (Vector2 mapPos);
        void saveAIPlayers (std::vector<AIPlayer> currentAIPlayers);
        std::vector<AIPlayer>* getAIPlayers ();
        Vector2 getSpawnLocation(int spawnIndex);
        std::unordered_map<int , MapSwitcherProp>* getMapSwitchersList();
        std::unordered_map<int, AutoMapSwitcherProp>* getAutoMapSwitcherList();
        int getCollisionCode();
        WorldEnums getWorldName();
        std::vector<Shop>* getCurrentWorldShops();
        std::vector<ShopItem>* getShopItems(std::string name);
};

#endif