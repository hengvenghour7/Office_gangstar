#include <vector>
#include "character.h"
#include "globalVar.h"
#include "drawing.h"
#include "map.h"
#include "UIHandler/UI.h"
#include "gameEnums.h"

class Game {
    private:
        bool isGameOver;
        bool isMenuOpen;
        bool isOpenInteractionDialog;
        bool isCanSwitch{true};
        float switchCooldownTime {0};
        const char* interactionText{""};
        GameStateEnums gameState;
        GameUIStateEnums gameUIState;
        ShopUI shopUI;
        UI gameUI;
        MapBoundary mapBoundary1;
        std::vector<AIPlayer> enemies{};
        AIPlayer NPC;
        std::vector<Prop> signProps{};
        std::vector<Prop> mapProps{};
        std::vector<Drawing*> allDrawableObjects;
        Vector2 speechLocation{SCREEN_WIDTH*0.5 - 320*0.5f, SCREEN_HEIGHT - (96*0.5 + 50)};
        Texture2D speechBackground = LoadTexture("resources/image/UI/speechUI.png");
        Texture2D mapTexture = LoadTexture("resources/image/office_gang_map.png");
        WorldDrawProperty worldDrawProperty;
        Vector2 pauseScreenWorldPos{0,0};
        WorldSet* currentWorld;
        Player player;
    public:
        Game();
        void tick(float deltaTime);
        void handleStartMenuTick();
        void startGame();
        void saveGame();
        void loadGame();
        void checkSwitchWorldInteraction(float deltaTime);
        void checkPropsInteraction(Player& player, Vector2 mapPos);
        void checkShopInteraction(Player& player, Vector2 mapPos);
        void loadWorld(WorldEnums targetMap, Vector2 targetLocation);
        void handleGamePlayUIInteraction();
        void prepareWorld(SpawnToDetail& spawnToDetail);
};