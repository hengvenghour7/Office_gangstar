#ifndef CHARACTER_H
#define CHARACTER_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "globalVar.h"
#include "mapCollision.h"
#include "helpers.h"
#include"drawing.h"

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
class Character : public Drawing
    {
    protected:
        float width;
        float height;
        Vector2 worldPos{static_cast<float>((rand() % 500) + 10), static_cast<float>((rand() % 200) + 100)};
        Vector2 screenPos{0,0};
        Vector2 direction{0,0};
        float speed;
        int startCols{0};
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
        float takeDamageTimeCap;
        float stunTimeCap;
        HealthComponent characterHealth;
        enum PlayerState playerState{Idle};
        enum PlayerState playerState2{Idle};
        enum PlayerDirection directionState;
        enum PlayerDirection oldDirectionState;
        bool isNeedResetCols{false};
        float damage;
    public:
        virtual void tick (float deltaTime);
        Character (const char * imageTexture, float speed, float damage);
        void updateAnimation (float deltaTime);
        virtual void draw (Vector2 des) override;
        void updateCharacterProgess (float deltaTime) {
            updateAnimation(deltaTime);
        }
        void updateHitBox ();
        void drawHealth ();
        void takeDamage (Character* secondCollider,float damage, float deltaTime);
        void setCharacterPos (Vector2 inputWorldPos, Vector2 playerPos);
        void updateDirectionState (Vector2 newDirection);
        void updatePlayerState (enum PlayerState state, bool specialUpdate = false);
        Vector2* getWorldPosPointer ();
        Rectangle getCharacterCollision();
        Rectangle getCharacterHitBox();
        void updateDirectionStateAI (Vector2 newDirection);
        enum PlayerState getPlayerState2 ();
        enum PlayerState getPlayerState () {
            return playerState;
        }
        float getDamage () {
            return damage;
        }
        HealthComponent getHealthComponent();
    };
class Player: public Character
{
    private:
        Vector2 direction{0,0};
        std::vector<std::vector<int>>* worldCollisionArray;
    public:
        Player (const char * imageTexture, std::vector<std::vector<int>>* worldCollisionArray, float speed, float damage);
        virtual void tick(float deltaTime) override;
        void drawHealth(int x, int y);
        Vector2 getWorldPos ();
        Vector2 getScreenPos ();
    };
    
class AIPlayer : public Character {
    private:
        Player* player;
        int id;
        bool isNeedToMoveBack{false};
        Vector2 direction;
        Rectangle collider{};
        // Vector2 currentDirection {0,0};
    public:
        AIPlayer (const char * imageTexture, Player* inputTarget, int id, float speed, float damage);
        void doDamage ();
        void AITick(float deltaTime, std::vector<AIPlayer>* allAIPlayer);
    void appraochTarget (std::vector<AIPlayer>* allAIPlayer, float deltaTime) ;
    void drawHealth() ;
};

#endif