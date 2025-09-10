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
        bool isTakeDamage{false};
        float takeDamageTimeCap = 0.f;
    public:
        virtual void tick (float deltaTime);
        Character (const char * imageTexture);
        void updateAnimation (float deltaTime);
        void drawImage ();
        void updateCharacterProgess (float deltaTime) {
            updateAnimation(deltaTime);
            drawImage();
        }
        void takeDamage (Character* secondCollider, Vector2 MapPos);
        
        Rectangle getCharacterCollision();
    };
class Player: public Character
{
    private:
        Vector2 direction{0,0};
        MapBoundary* boundary{};
    public:
        Player (const char * imageTexture, MapBoundary* inputBoundary);
        virtual void tick(float deltaTime) override;
        Vector2* getWorldPos () {
            return &worldPos;
        }
    };
    
class AIPlayer : public Character {
    private:
        Vector2* playerWorldPos;
        Player* target;
    public: 
        AIPlayer (const char * imageTexture, Vector2* inputPlayerWorldPos, Player* inputTarget);
        virtual void tick(float deltaTime) override;
        AIPlayer (const char * imageTexture) : Character(imageTexture) {
    }
    void appraochTarget () ;
    void setAIPos (Vector2 worldPos);
};