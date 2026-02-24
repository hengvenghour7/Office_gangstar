#include "collisionData.h"
#include "mapCollision.h"
#include "map.h"
#include "mapData.h"
#include "globalVar.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

WorldSet& getCenterWorld(Player& player) {
    static std::vector<PropDrawCondition> animatedObjectPropCondition{
        {79735, LoadTexture("resources/image/Fishing_Boat_2_left.png"), 0, 0, 8, 0, 768/8, 64},
        {79736, LoadTexture("resources/image/Fishing_Boat_left_2.png"), 0, 0, 8, 0, 640/8,48}
    };
    static std::vector<PropDrawCondition> arrowPropCondition{
    {79730, LoadTexture("resources/image/Modern_UI_Style_1.png") , 8, 5, 10, 0, 240/15, 688/21}
    };
    static std::vector<PropDrawCondition> characterPropCondition{
        {79735, LoadTexture("resources/image/character/woman.png"), 0, 8, 4, 0, 896/56, 640/20, {true, 79731}}
    };
    static std::vector<PropDrawCondition> carPropCondition{
        {79735, LoadTexture("resources/image/Buses_2.png"), 0, 1, 3, 0, 2272/15.8, 1200/11}
    };
    static std::vector<PropDrawCondition> interactablePropCondition{
        {79737, LoadTexture("resources/image/Beach_Umbreall_Open.png"), 0, 0, 7, 0, 384/8, 304/4.5}
    };

    static MapProp boatProp(&animatedObjectLocation, 150, 100, 16, &animatedObjectPropCondition, MAP_SCALE);
    static MapProp arrowProp(&arrowLocationData, 150, 100, 16, &arrowPropCondition, MAP_SCALE);
    static MapProp carProp(&vehiclePath, 150, 100, 16, &carPropCondition, MAP_SCALE);
    static MapProp characterProp(&characterPath, 150, 100, 16, &characterPropCondition, MAP_SCALE);

    static std::vector<MapProp*> centerWorldProps = {
        &boatProp, &arrowProp, &carProp, &characterProp
    };
    std::vector<DrawingDataSet> drawingDataSet = {};
    static WorldSet centerWorld(
        "resources/image/office_gang_map.png",
        "resources/image/office_gang_map_3.png",
        drawingDataSet,
        150,
        100,
        &collisionData,
        &centerWorldProps,
        "resources/maps/office_gang_map.tmj",
        WorldEnums::CenterWorld,
        player
    );

    return centerWorld;
}
WorldSet& getMarketInterior(Player& player) {

    static std::vector<MapProp*> marketInteriorProps = {
    };
    std::vector<DrawingDataSet> drawingDataSet = {};
    static WorldSet marketInterior(
        "resources/image/supermarket_map.png",
        "",
        drawingDataSet,
        50,
        20,
        &supermarketBoundary,
        &marketInteriorProps,
        "resources/maps/supermarket_map.tmj",
        WorldEnums::InteriorSuperMarket,
        player
    );

    return marketInterior;
}
WorldSet& getOfficeInterior(Player& player) {
    static std::vector<MapProp*> officeInteriorProps = {
    };
    std::vector<int> mapCollisionData;
    std::fstream file("resources/maps/office_interior.tmj");
    if (file.is_open()) {
        json j;
        file >> j;
        mapCollisionData = getArrayFromJson(j, "collision");
    }
    std::vector<DrawingDataSet> drawingDataSet = {};
    static WorldSet officeInterior(
        "resources/image/office_interior.png",
        "",
        drawingDataSet,
        10,
        20,
        &mapCollisionData,
        &officeInteriorProps,
        "resources/maps/office_interior.tmj",
        WorldEnums::InteriorOffice1,
        player,
        2
    );

    return officeInterior;
}
WorldSet& getOfficeInterior2(Player& player) {
    static std::vector<MapProp*> officeInteriorProps = {
    };
    std::vector<int> mapCollisionData;
    std::fstream file("resources/maps/office_interior_2.tmj");
    if (file.is_open()) {
        json j;
        file >> j;
        mapCollisionData = getArrayFromJson(j, "collision");
    }
    std::vector<DrawingDataSet> drawingDataSet = {};
    static WorldSet officeInterior2(
        "resources/image/office_interior_2.png",
        "resources/image/office_interior_2_top.png",
        drawingDataSet,
        40,
        20,
        &mapCollisionData,
        &officeInteriorProps,
        "resources/maps/office_interior_2.tmj",
        WorldEnums::InteriorOffice2,
        player
    );

    return officeInterior2;
}
WorldSet& getHardwareInterior(Player& player) {
    static std::vector<MapProp*> hardwareInteriorProps = {
    };
    std::vector<int> mapCollisionData;
    std::fstream file("resources/maps/hardware_interior.tmj");
    if (file.is_open()) {
        json j;
        file >> j;
        mapCollisionData = getArrayFromJson(j, "collision");
    }
    std::vector<DrawingDataSet> drawingDataSet = {};
    static WorldSet hardwareInterior(
        "resources/image/sunna_interior.png",
        "",
        drawingDataSet,
        20,
        20,
        &mapCollisionData,
        &hardwareInteriorProps,
        "resources/maps/sunna_interior.tmj",
        WorldEnums::InteriorHardware,
        player
    );

    return hardwareInterior;
}
WorldSet& getSunnaInterior(Player& player) {
    static std::vector<MapProp*> sunnaInteriorProps = {
    };
    std::vector<int> mapCollisionData;
    std::fstream file("resources/maps/sunna_interior.tmj");
    if (file.is_open()) {
        json j;
        file >> j;
        mapCollisionData = getArrayFromJson(j, "collision");
    }
    std::vector<DrawingDataSet> drawingDataSet = {};
    static WorldSet sunnaInterior(
        "resources/image/sunna_interior.png",
        "",
        drawingDataSet,
        20,
        30,
        &mapCollisionData,
        &sunnaInteriorProps,
        "resources/maps/sunna_interior.tmj",
        WorldEnums::InteriorSunna,
        player
    );

    return sunnaInterior;
}
WorldSet& getUpperMap(Player& player) {
    static std::vector<MapProp*> upperMapProps = {
    };
    std::vector<int> mapCollisionData;
    std::fstream file("resources/maps/upper_map.tmj");
    if (!file.is_open()) {
    }
    if (file.is_open()) {
        json j;
        file >> j;
        mapCollisionData = getArrayFromJson(j, "Collision");
    }
    std::vector<DrawingDataSet> drawingDataSet = {
        {"resources/image/upper_map_2.png", 1}
    };
    static WorldSet upperMap(
        "resources/image/upper_map.png",
        "",
        drawingDataSet,
        150,
        100,
        &mapCollisionData,
        &upperMapProps,
        "resources/maps/upper_map.tmj",
        WorldEnums::UpperMap,
        player,
        3,
        5
    );

    return upperMap;
}
WorldSet& getWorld(WorldEnums world, Player& player) {
    switch (world)
    {
    case CenterWorld:
        return getCenterWorld(player);
    case InteriorSuperMarket: 
        return getMarketInterior(player);
    case InteriorOffice1:
        return getOfficeInterior(player);
    case InteriorOffice2:
        return getOfficeInterior2(player);
    case InteriorHardware:
        return getHardwareInterior(player);
    case InteriorSunna:
        return getSunnaInterior(player);
    case WorldEnums::UpperMap:
        return getUpperMap(player);
    default:
        return getCenterWorld(player);
        break;
    }
}