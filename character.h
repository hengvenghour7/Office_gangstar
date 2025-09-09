#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "globalVar.h"
#include "mapCollision.h"

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
        virtual void tick (float deltaTime);
        Character (const char * imageTexture);
        void updateAnimation (float deltaTime);
        void drawImage ();
        void updateCharacterProgess (float deltaTime) {
            drawImage();
            updateAnimation(deltaTime);
        }
        Rectangle getCharacterCollision();
    };
class Player: public Character
{
    private:
        Vector2 direction{0,0};
        MapBoundary* boundary{};
    public:
        virtual void tick(float deltaTime) override;
        Player (const char * imageTexture, MapBoundary* inputBoundary);
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
        // virtual void tick(float deltaTime) override;
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