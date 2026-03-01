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
#include "item/item.h"
#include <functional>

struct SpawnToDetail {
    WorldEnums targetMap;
    int targetSpawnPoint;
};
struct DrawingDataSet {
    const char* imgSrc;
    int level;
};
struct InteractableInputProperties {
    std::string name;
    std::function<void()> func;
};
struct LevelData {
    std::vector<std::vector<int>> collisionArray;
    int collisionCode = 1;
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
class InteractablePropV2: public Drawing {
    Rectangle dimension;
    std::function<void()> func;
    Texture2D imgTexture;
    int startFrame;
    int midFrame;
    int endFrame;
    bool isInteracted {false};
    bool isActionFinished {true};
    bool isContinueAnimation {false};
    int currentFrame;
    int srcWidth;
    int srcHeight;
    float animationUpdateTime{0};
    int interactableDistance;
    public:
        InteractablePropV2 (Rectangle dimension, std::function<void()> function, std::string imgSrc, 
        int startFrame, int midFrame, int endFrame, int srcWidth, int srcHeight, int interactableDistance);
        virtual void draw (Vector2 mapPos) override;
        void updateAnimation (float deltaTime);
        void handleInteraction();
        void doFunction();
        Vector2 getCenter(Vector2& mapPos);
        Rectangle getDimension();
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
class LevelSwitcher: public Drawing {
    public:
        int option1;
        int option2;
        virtual void draw(Vector2 mapPos) override;
        Rectangle getScreenPosDimension(Vector2 mapPos);
        Rectangle dimension;
        LevelSwitcher(int option1, int option2, Rectangle dimension);
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
class MapLayer : public Drawing {
    Texture2D layerTexture;

    public:
    MapLayer(const char* imgSrc, int level);
    virtual void draw(Vector2 mapPos) override;
};
class WorldSet {
    std::string worldPropertySrc;
    std::unordered_map<int , MapSwitcherProp> mapSwitchersList{};
    std::unordered_map<int , AutoMapSwitcherProp> autoMapSwitcherList{};
    std::unordered_map<int , LevelSwitcher> autoLevelSwitcherList{};
    std::vector<AIPlayer> AIPlayers;
    int collisionCode{1};
    WorldEnums worldName;
    std::vector<Shop> shops;
    std::unordered_map<int , LevelData> levelDataList{};
    std::vector<InteractableItem> interactableItemList{};
    std::vector<MapLayer> mapLayers{};
    std::vector<InteractablePropV2> interactableV2List{};

    public:
        WorldSet(const char* backgroundTexture, const char* foregroundTexture, std::vector<DrawingDataSet> drawingDataSet, int mapWidth, int mapHeight, 
        std::vector<int>* collisionData, std::vector<MapProp*>* worldProps, std::string mapPropertyPath, 
        WorldEnums worldName, Player& player, std::vector<InteractableInputProperties> interactableProperties, int levelAmount = 0, int AI_amount = 0);
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
        std::unordered_map<int , LevelSwitcher>* getAutoLevelSwitcherList();
        int getCollisionCode();
        WorldEnums getWorldName();
        std::vector<Shop>* getCurrentWorldShops();
        std::vector<InteractableItem>* getInteractableItem();
        void handleItemPickUp(Player& player, Vector2 mapPos);
        std::vector<ShopItem>* getShopItems(std::string name);
        void addItemtoWorld(InteractableItem item);
        std::unordered_map<int , LevelData>* getLevelDataList();
        std::vector<Drawing*> getMapLayers();
        std::vector<InteractablePropV2>* getInteractableV2List();
};

#endif