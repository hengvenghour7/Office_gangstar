#include <vector>
#include "character.h"
#include "globalVar.h"
#include "drawing.h"
#include "map.h"

class Game {
    private:
        bool isGameOver;
        MapBoundary mapBoundary1;
        std::vector<AIPlayer> enemies{};
        AIPlayer NPC;
        std::vector<Prop> signProps{};
        std::vector<Prop> mapProps{};
        std::vector<Drawing*> allDrawableObjects;
        Texture2D mapTexture = LoadTexture("resources/image/office_gang_map.png");
        WorldDrawProperty worldDrawProperty;
        WorldHandler worldHandler;
        Player player;
        std::vector<PropDrawCondition> arrowPropCondition{
        {79730, LoadTexture("resources/image/Modern_UI_Style_1.png") , 8, 5, 10, 0, 240/15, 688/21}
        };
        std::vector<PropDrawCondition> animatedObjectPropCondition{
            {79735, LoadTexture("resources/image/Fishing_Boat_2_left.png"), 0, 0, 8, 0, 768/8, 64},
            {79736, LoadTexture("resources/image/Fishing_Boat_left_2.png"), 0, 0, 8, 0, 640/8, 48}
        };
        std::vector<PropDrawCondition> characterPropCondition{
            {79735, LoadTexture("resources/image/character/woman.png"), 0, 8, 4, 0, 896/56, 640/20}
        };
        std::vector<PropDrawCondition> carPropCondition{
            {79735, LoadTexture("resources/image/Buses_2.png"), 0, 1, 3, 0, 2272/15.8, 1200/11}
        };
        std::vector<PropDrawCondition> interactablePropCondition{
            {79737, LoadTexture("resources/image/Beach_Umbreall_Open.png"), 0, 0, 7, 0, 384/8, 304/4.5}
        };
        MapProp arrowProp;
        MapProp boatProp;
        MapProp characterProp;
        MapProp carProp;

    public:
        Game();
        void tick(float deltaTime);
};