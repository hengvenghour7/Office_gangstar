#include "collisionData.h"
#include "mapCollision.h"
#include "map.h"
#include "mapData.h"
#include "globalVar.h"

WorldSet& getCenterWorld() {
    static std::vector<PropDrawCondition> animatedObjectPropCondition{
        {79735, LoadTexture("resources/image/Fishing_Boat_2_left.png"), 0, 0, 8, 0, 768/8, 64},
        {79736, LoadTexture("resources/image/Fishing_Boat_left_2.png"), 0, 0, 8, 0, 640/8,48}
    };
    static std::vector<PropDrawCondition> arrowPropCondition{
    {79730, LoadTexture("resources/image/Modern_UI_Style_1.png") , 8, 5, 10, 0, 240/15, 688/21}
    };
    static std::vector<PropDrawCondition> characterPropCondition{
        {79735, LoadTexture("resources/image/character/woman.png"), 0, 8, 4, 0, 896/56, 640/20}
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

    static WorldSet centerWorld(
        "resources/image/office_gang_map.png",
        "resources/image/office_gang_map_3.png",
        150,
        100,
        &collisionData,
        &centerWorldProps,
        "resources/maps/office_gang_map.tmj"
    );

    return centerWorld;
}
WorldSet& getMarketInterior() {

    static std::vector<MapProp*> marketInteriorProps = {
    };

    static WorldSet centerWorld(
        "resources/image/supermarket_map.png",
        "",
        50,
        20,
        &supermarketBoundary,
        &marketInteriorProps,
        "resources/maps/supermarket_map.tmj"
    );

    return centerWorld;
}
WorldSet& getWorld(WorldEnums world) {
    switch (world)
    {
    case CenterWorld:
        return getCenterWorld();
    case InteriorSuperMarket: 
        return getMarketInterior();
    default:
        return getCenterWorld();
        break;
    }
}