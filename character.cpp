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
            characterRecDes = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
            characterCollision = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
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
void Character::drawImage () {
            characterCollision = {characterRecDes.x, characterRecDes.y+12, width*scale_facter, (height-6)*scale_facter};
            // DrawRectangle(characterCollision.x, characterCollision.y, characterCollision.width, characterCollision.height, YELLOW);
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
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
            characterRecDes = {screenPos.x, screenPos.y, width*scale_facter, height*scale_facter};
            characterCollision = {screenPos.x, screenPos.y+12, width*scale_facter, (height-6)*scale_facter};
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
                                worldPos = Vector2Add(worldPos, Vector2Normalize(direction)*speed);
                            }
                    direction = {0,0};
                }
                Character::tick(deltaTime);
            }
AIPlayer::AIPlayer (const char * imageTexture, Vector2* inputPlayerWorldPos): Character(imageTexture) {
    playerWorldPos = inputPlayerWorldPos;
}
void AIPlayer::tick(float deltaTime) {
    setAIPos(Vector2Scale(*playerWorldPos, -1.f));
    Character::tick(deltaTime);
}
void AIPlayer::setAIPos (Vector2 worldPos) {
        characterRecDes = {worldPos.x, worldPos.y, width*2, height*2};
    }