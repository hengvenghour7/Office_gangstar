#ifndef CHARACTER_H
#define CHARACTER_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "globalVar.h"
#include "mapCollision.h"
#include <cstdlib>
#include <ctime>

using namespace std;

class HealthComponent {
    protected:
    public:
        float maxHealth{};
        float currentHealth{};
        Vector2 healthDes;
        HealthComponent (float inputMaxHealth);
        void takeDamage(float damage);
        void heal(float healAmount);
        void drawHealth(float locationX, float locationY, float width, float height, Color inputColor);
};
class Character
    {
    protected:
        float width;
        float height;
        // int r = rand() % 10;
        Vector2 worldPos{static_cast<float>((rand() % 100) + 100), static_cast<float>((rand() % 200) + 100)};
        Vector2 screenPos{0,0};
        Vector2 direction{0,0};
        float speed{1};
        float AISpeed{static_cast<float>(rand() % 2 + 0.5)};
        int maxCols;
        int colIndex;
        int rowIndex;
        float scale_factor{1.5f};
        float updateAnimationTime = 0;
        Rectangle characterRecDes;
        Rectangle characterRecSrc;
        Image characterImg;
        Texture2D characterTexture;
        Rectangle characterCollision;
        Rectangle characterHitBox;
        bool isTakeDamage{false};
        bool isAttacking{false};
        float takeDamageTimeCap{1.f};
        float takeDamageAnimationTime{0.5f};
        HealthComponent characterHealth;
    public:
        virtual void tick (float deltaTime);
        Character (const char * imageTexture);
        void updateAnimation (float deltaTime);
        void drawImage ();
        void updateCharacterProgess (float deltaTime) {
            updateAnimation(deltaTime);
            drawImage();
        }
        void drawHealth ();
        void takeDamage (Character* secondCollider, Vector2 MapPos, float deltaTime);
        void takeDamage2 (Character* secondCollider, Vector2 MapPos, float deltaTime);
        void setCharacterPos (Vector2 inputWorldPos, Vector2 playerPos);
        Vector2* getWorldPosPointer ();
        Rectangle getCharacterCollision();
        Rectangle getCharacterHitBox();
    };
class Player: public Character
{
    private:
        Vector2 direction{0,0};
        MapBoundary* boundary{};
    public:
        Player (const char * imageTexture, MapBoundary* inputBoundary);
        virtual void tick(float deltaTime) override;
        Vector2 getWorldPos ();
        Vector2 getScreenPos ();
    };
    
class AIPlayer : public Character {
    private:
        Player* player;
    public: 
        AIPlayer (const char * imageTexture, Player* inputTarget);
        virtual void tick(float deltaTime) override;
    void appraochTarget () ;
    void drawHealth() ;
};

#endif