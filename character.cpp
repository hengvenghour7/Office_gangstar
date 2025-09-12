#include "character.h"
#include "mapCollision.h"
#include "helpers.h"


Character::Character (const char * imageTexture) {
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
            characterCollision = {screenPos.x, (*(&screenPos)).y, width*scale_factor, height*scale_factor};
        }
void Character::takeDamage (Character* secondCollider, Vector2 MapPos) {
            if (checkIsCollide(getCharacterCollision(), secondCollider->getCharacterCollision(), MapPos, 0, 0).isCollide) {
                takeDamageTimeCap = 0;
                isTakeDamage = true;
                return;
            }
            isTakeDamage = false
            ;
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
    }
void Character::drawImage () {
            // characterCollision = {characterRecDes.x, characterRecDes.y+12, width*scale_factor, (height-6)*scale_factor};
            // DrawRectangle(characterCollision.x, characterCollision.y, characterCollision.width, characterCollision.height, YELLOW);
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
Player::Player (const char * imageTexture, MapBoundary* inputBoundary): Character(imageTexture) {
            boundary = inputBoundary;
            screenPos.x = SCREEN_WIDTH/2;
            screenPos.y = SCREEN_HEIGHT/2;
            worldPos = {-100, 0};
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterCollision = {screenPos.x, screenPos.y+12, width*scale_factor, (height-6)*scale_factor};
        }
void Player::tick (float deltaTime) {
            if (isTakeDamage) rowIndex = 19;
            else {

                    if (direction.x == 0 && direction.y == 0) rowIndex = 1;
                    if (IsKeyDown(KEY_K)) rowIndex = 13;
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
void AIPlayer::appraochTarget () {
    Vector2 direction = Vector2Normalize(Vector2Subtract(player->getScreenPos(), screenPos)) ;
    worldPos = Vector2Add(worldPos,direction);
    setCharacterPos(worldPos, player->getWorldPos());
    }