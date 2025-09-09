#include "character.h"


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
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
void Character::tick (float deltaTime) {
            if (IsKeyDown(KEY_K)) rowIndex = 13;
            else {
                rowIndex = 2;
                if (IsKeyDown(KEY_A)) {
                    direction.x = -1;
                }
                if (IsKeyDown(KEY_D)) {
                    direction.x = 1;
                }
                if (IsKeyDown(KEY_W)) direction.y = -1;
                if (IsKeyDown(KEY_S)) direction.y = 1;
            }
            if(Vector2Length(direction) != 0) {
                worldPos = Vector2Subtract(worldPos, Vector2Normalize(direction)*speed);
            }
            direction = {0,0};
        }
Rectangle Character::getCharacterCollision ()  {
            return characterCollision;
        };