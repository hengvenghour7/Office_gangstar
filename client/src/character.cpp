#include "character.h"
#include "mapCollision.h"
#include "helpers.h"

HealthComponent::HealthComponent (float inputMaxHealth) {
    maxHealth = inputMaxHealth;
    currentHealth = maxHealth;
    healthDes = {0, 0};
};
void HealthComponent::takeDamage(float damage) {
    // cout<< "take damage current health is" << currentHealth;
    currentHealth-= damage;
    // DrawRectangle(healthDes.x, healthDes.y, currentHealth, 10, GREEN);
};
void HealthComponent::heal(float healAmount) {
    currentHealth+= healAmount;
};
void HealthComponent::drawHealth(float locationX, float locationY, float width, float height, Color inputColor){
    DrawRectangle(locationX, locationY, width, height, inputColor);
};
// void HealthComponent::drawHealth (int locationX, int locationY, float width, float height) {
//     DrawRectangle(locationX, locationY, width, height, GREEN);
// };
Character::Character (const char * imageTexture) : characterHealth(300) {
            width = 896/56;
            height = 640/20;
            maxCols = 6;
            colIndex = 0;
            rowIndex = 2;
            speed = 2;
            characterImg = LoadImage(imageTexture);
            characterTexture = LoadTextureFromImage(characterImg);
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            characterCollision = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterHitBox = {screenPos.x + 20, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Character::takeDamage (Character* secondCollider, Vector2 MapPos, float deltaTime) {
            if (checkIsCollide(getCharacterCollision(), secondCollider->getCharacterCollision(), MapPos, 0, 0).isCollide && takeDamageTimeCap >= 1.5f) {
                characterHealth.takeDamage(20);
                takeDamageTimeCap = 0;
                takeDamageAnimationTime = 0;
                isTakeDamage = true;
                return;
            }
            // if (isTakeDamage) {
            // }
            takeDamageTimeCap += deltaTime;
            takeDamageAnimationTime += deltaTime;
            if (takeDamageAnimationTime > 0.5) {
                isTakeDamage = false;
            }
            if (isTakeDamage) rowIndex = 19;
        }
void Character::takeDamage2 (Character* secondCollider, Vector2 MapPos, float deltaTime) {
            if (checkIsCollide(getCharacterHitBox(), secondCollider->getCharacterHitBox(), MapPos, 0, 0).isCollide && takeDamageTimeCap >= 1.5f && secondCollider->isAttacking) {
                characterHealth.takeDamage(20);
                takeDamageTimeCap = 0;
                takeDamageAnimationTime = 0;
                isTakeDamage = true;
                return;
            }
            takeDamageTimeCap += deltaTime;
            takeDamageAnimationTime += deltaTime;
            if (takeDamageAnimationTime > 0.5) {
                isTakeDamage = false;
            }
            if (isTakeDamage) {
                rowIndex= 19;
            } 
            else {
                rowIndex=2;
            }
        }
void Character::updateAnimation (float deltaTime) {
            updateAnimationTime += deltaTime;
            if (updateAnimationTime > 0.1) {
                updateAnimationTime = 0;
                colIndex++;
                if (colIndex > maxCols - 1) colIndex = 0;
                characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            }
        }
void Character::setCharacterPos(Vector2 inputWorldPos, Vector2 playerPos) {
        worldPos = inputWorldPos;
        screenPos = Vector2Subtract(worldPos, playerPos);
        characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
        characterCollision = {characterRecDes.x, characterRecDes.y+12, width*scale_factor, (height-6)*scale_factor};
        characterHitBox = {screenPos.x + 20, screenPos.y, width*scale_factor, height*scale_factor};
    }
void Character::drawHealth () {
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y, characterHealth.currentHealth, 10, GREEN);
}
void Character::drawImage () {
            // characterCollision = {characterRecDes.x, characterRecDes.y+12, width*scale_factor, (height-6)*scale_factor};
            // DrawRectangle(characterHitBox.x, characterHitBox.y, characterHitBox.width, characterHitBox.height, YELLOW);
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
Vector2* Character::getWorldPosPointer () {
    return &worldPos;
}
void Character::tick (float deltaTime) {
            updateAnimation(deltaTime);
            drawImage();
        }
        Rectangle Character::getCharacterCollision ()  {
            return characterCollision;
        };
        Rectangle Character::getCharacterHitBox ()  {
            return characterHitBox;
        };
Player::Player (const char * imageTexture, MapBoundary* inputBoundary): Character(imageTexture) {
            boundary = inputBoundary;
            screenPos.x = SCREEN_WIDTH/2;
            screenPos.y = SCREEN_HEIGHT/2;
            worldPos = {-100, 1500};
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterCollision = {screenPos.x, screenPos.y+12, width*scale_factor, (height-6)*scale_factor};
            characterHitBox = {screenPos.x + 30, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Player::tick (float deltaTime) {
            if (!isTakeDamage) {
                if (direction.x == 0 && direction.y == 0) {
                    rowIndex = 1;
                    isAttacking = false;
                }
                if (IsKeyDown(KEY_K)) {
                    rowIndex = 13;
                    isAttacking = true;
                }
                else {
                    if (IsKeyDown(KEY_A)) {
                        direction.x = -1;
                    }
                    if (IsKeyDown(KEY_D)) {
                        direction.x = 1;
                    }
                    if (IsKeyDown(KEY_W)) direction.y = -1;
                    if (IsKeyDown(KEY_S)) direction.y = 1;
                    if (direction.x != 0 || direction.y != 0) rowIndex = 2;
                    isAttacking = false;
                }
                if(Vector2Length(direction) != 0 && !boundary->checkBoundaryCollision(characterCollision, worldPos, direction.x*speed, direction.y*speed).isCollide) {
                    // Vector2 worldPostNegative = Vector2Scale(Vector2Add(worldPos, Vector2Normalize(direction)*speed), -1);
                    // if (!(worldPostNegative.x > 0 || worldPostNegative.y > 0)) {
                        worldPos = Vector2Add(worldPos, Vector2Normalize(direction)*speed);
                    // } else {
                    //     screenPos = Vector2Add(screenPos, Vector2Normalize(direction)*speed);
                    // }
                        }
                direction = {0,0};
            } 
                Character::tick(deltaTime);
            }
Vector2 Player::getWorldPos () {
            return worldPos;
        }
Vector2 Player::getScreenPos () {
            return screenPos;
        };
AIPlayer::AIPlayer (const char * imageTexture, Player* inputPlayer): Character(imageTexture) {
    player = inputPlayer;
}
void AIPlayer::tick(float deltaTime) {
    appraochTarget();
    Character::tick(deltaTime);
}
void AIPlayer::drawHealth() {
    characterHealth.healthDes = {characterCollision.x, characterCollision.y};
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y - 20, characterHealth.currentHealth/4, 10, RED);
}
void AIPlayer::appraochTarget () {
    Vector2 direction = Vector2Normalize(Vector2Subtract(player->getScreenPos(), screenPos)) ;
    if(!isTakeDamage) worldPos = Vector2Add(worldPos,direction);
    setCharacterPos(worldPos, player->getWorldPos());
    }