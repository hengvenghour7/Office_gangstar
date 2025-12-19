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
        WorldSet* currentWorld;
        Player player;
    public:
        Game();
        void tick(float deltaTime);
        void checkSwitchWorldInteraction(Player& player);
};