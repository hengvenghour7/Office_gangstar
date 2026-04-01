#include <vector>
#include "character.h"
#include "globalVar.h"
#include "drawing.h"
#include "map.h"
#include "UIHandler/UI.h"
#include "gameEnums.h"

enum class DayState {
    AM,
    PM,
};
class Time {
    int hour;
    int minute;
    float countDownTime;
    DayState dayState;
    public:
    Time();
    void draw();
    void tick(float deltaTime);
    DayState& getDayState ();
    int getHour();
    int getMinute();
};
class Game {
    private:
        bool isWorldInitialize{false};
        bool isGameOver;
        bool isMenuOpen;
        bool isOpenInteractionDialog;
        bool isCanSwitch{true};
        float switchCooldownTime {0};
        const char* interactionText{""};
        Vector2 cameraShakeVector{0, 0};
        float shakeAmplitudeFactor{1.0};
        GameStateEnums gameState;
        GameUIStateEnums gameUIState;
        ShopUI shopUI;
        UI gameUI;
        MapBoundary mapBoundary1;
        std::vector<AIPlayer> enemies{};
        WorldSet* currentWorld;
        AIPlayer NPC;
        std::vector<Prop> signProps{};
        std::vector<Prop> mapProps{};
        std::vector<Drawing*> allDrawableObjects;
        Vector2 speechLocation{SCREEN_WIDTH*0.5 - 320*0.5f, SCREEN_HEIGHT - (96*0.5 + 50)};
        Texture2D speechBackground = LoadTexture("resources/image/UI/speechUI.png");
        Texture2D mapTexture = LoadTexture("resources/image/office_gang_map.png");
        WorldDrawProperty worldDrawProperty;
        Vector2 pauseScreenWorldPos{0,0};
        Player player;
        Time time;
        RenderTexture2D lightMask;
    public:
        Game();
        void tick(float deltaTime);
        void handleStartMenuTick();
        void startGame();
        void saveGame();
        void loadGame();
        void checkSwitchWorldInteraction(float deltaTime, Vector2& mapPos);
        void checkPropsInteraction(Player& player, Vector2 mapPos);
        void checkShopInteraction(Player& player, Vector2 mapPos);
        void loadWorld(WorldEnums targetMap, Vector2 targetLocation);
        void handleGamePlayUIInteraction();
        void prepareWorld(SpawnToDetail& spawnToDetail);
        Vector2 handleCameraShake();
};