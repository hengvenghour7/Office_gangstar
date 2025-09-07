#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"

using namespace std;

int main () {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float MAP_SCALE = 1.5;
    Vector2 mapPos = {0,0};
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
    class Character
    {
    protected:
        float width;
        float height;
        float x;
        float y;
        float desX;
        float desY;
        float speed;
        int maxCols;
        int colIndex;
        int rowIndex;
        float scale_facter{2.f};
        float updateAnimationTime = 0;
        Rectangle characterRecDes;
        Rectangle characterRecSrc;
        Image characterImg;
        Texture2D characterTexture;
        Boundary characterCollision;
    public:
        Character (const char * imageTexture) {
            width = 896/56;
            height = 640/20;
            x = 0;
            y = 0;
            desX = 0;
            desY = 0;
            maxCols = 6;
            colIndex = 0;
            rowIndex = 2;
            speed = 2;
            characterImg = LoadImage(imageTexture);
            characterTexture = LoadTextureFromImage(characterImg);
            characterRecDes = {desX, desY, width*scale_facter, height*scale_facter};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            characterCollision = {(int)desX, (int)desY, width*scale_facter, height*scale_facter};
        }
        void updateAnimation (float deltaTime) {
            updateAnimationTime += deltaTime;
            if (updateAnimationTime > 0.1) {
                updateAnimationTime = 0;
                colIndex++;
                if (colIndex > maxCols - 1) colIndex = 0;
                characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            }
        }
        void drawImage () {
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
        void tryLog () {
            cout << "creating enw world";
        }
        void updateCharacterProgess (float deltaTime) {
            drawImage();
            updateAnimation(deltaTime);
        }
        Boundary getCharacterCollision() {
            return characterCollision;
        };
    };
    class Player: public Character
    {
    private:
        Vector2 worldPos{0,0};
        Vector2 direction{0,0};
    public:
        Player (const char * imageTexture): Character(imageTexture) {
            desX = SCREEN_WIDTH/2;
            desY = SCREEN_HEIGHT/2;
            characterRecDes = {desX, desY, width*scale_facter, height*scale_facter};
            characterCollision = {(int)desX, (int)desY, width*scale_facter, height*scale_facter};

        }
        void updateCharacterMovement () {
            if (IsKeyDown(KEY_A)) {
                direction.x = -1;
            }
            if (IsKeyDown(KEY_D)) {
                direction.x = 1;
            }
            if (IsKeyDown(KEY_W)) direction.y = -1;
            if (IsKeyDown(KEY_S)) direction.y = 1;
            if(Vector2Length(direction) != 0) {
                worldPos = Vector2Subtract(worldPos, Vector2Normalize(direction)*speed);
            }
            direction = {0,0};
        }
        void updateCharacterProgess (float deltaTime) {
            drawImage();
            updateAnimation(deltaTime);
            updateCharacterMovement();
        }
        Vector2 getWorldPos () {
            return worldPos;
        }
    };
    
    class AIPlayer : public Character {
        private:
            
        
        public: 
            AIPlayer (const char * imageTexture) : Character(imageTexture) {
            
        }
        void updateCharacterProgess (float deltaTime) {
            drawImage();
            updateAnimation(deltaTime);
        }
        void setAIPos (Vector2 mapPos) {
            characterRecDes = {mapPos.x, mapPos.y, width*2, height*2};
        }
    };
    Player player1("resources/image/character/workingman2.png");
    AIPlayer character2("resources/image/character/workingman.png");
    while (WindowShouldClose() == false){
        cout << "jjjjjj" << mapCollision.getWidth();
        float deltaTime = GetFrameTime();
        mapPos = player1.getWorldPos();
        Image mapImg = LoadImage("resources/image/office_gang_map.png");
        Texture2D mapTexture = LoadTextureFromImage(mapImg);
        BeginDrawing();
        ClearBackground(BLACK);
        // DrawTexture(texture, 0, 0, WHITE);
        DrawTextureEx(mapTexture, mapPos, 0,MAP_SCALE,WHITE);
        mapCollision.drawBoundary(MAP_SCALE, mapPos);
        mapCollision.checkBoundaryCollision(player1.getCharacterCollision(), mapPos);
        // DrawRectangle (272,200, 16, 16, RED);
        player1.updateCharacterProgess(deltaTime);
        character2.setAIPos(mapPos);
        character2.updateCharacterProgess(deltaTime);
        UnloadImage(mapImg);
            // Free the image, texture now lives on GPU
        EndDrawing();
    }

    CloseWindow();
}
