#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "globalVar.h"

using namespace std;

class Character
    {
    protected:
        float width;
        float height;
        Vector2 worldPos{0,0};
        Vector2 screenPos{0,0};
        Vector2 direction{0,0};
        float speed{1};
        int maxCols;
        int colIndex;
        int rowIndex;
        float scale_facter{2.f};
        float updateAnimationTime = 0;
        Rectangle characterRecDes;
        Rectangle characterRecSrc;
        Image characterImg;
        Texture2D characterTexture;
        Rectangle characterCollision;
    public:
        Character (const char * imageTexture) {
            width = 896/56;
            height = 640/20;
            maxCols = 6;
            colIndex = 0;
            rowIndex = 2;
            speed = 2;
            characterImg = LoadImage(imageTexture);
            characterTexture = LoadTextureFromImage(characterImg);
            characterRecDes = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            characterCollision = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
        }
        void updateAnimation (float deltaTime);
        void drawImage ();
        void updateCharacterProgess (float deltaTime) {
            drawImage();
            updateAnimation(deltaTime);
        }
        Rectangle getCharacterCollision();
        void tick (float deltaTime);
    };
    class Player: public Character
    {
    private:
        Vector2 worldPos{0,0};
        Vector2 direction{0,0};
    public:
        Player (const char * imageTexture): Character(imageTexture) {
            screenPos.x = SCREEN_WIDTH/2;
            screenPos.y = SCREEN_HEIGHT/2;
            characterRecDes = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
            characterCollision = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
        }
        void updateCharacterProgess (float deltaTime) {
            drawImage();
            updateAnimation(deltaTime);
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