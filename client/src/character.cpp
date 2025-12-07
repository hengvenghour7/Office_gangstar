#include "character.h"
#include "mapCollision.h"
#include "helpers.h"

HealthComponent::HealthComponent (float inputMaxHealth) {
    maxHealth = inputMaxHealth;
    currentHealth = maxHealth;
    healthDes = {0, 0};
};
void HealthComponent::takeDamage(float damage) {
    currentHealth-= damage;
};
void HealthComponent::heal(float healAmount) {
    currentHealth+= healAmount;
};
void HealthComponent::drawHealth(float locationX, float locationY, float width, float height, Color inputColor){
    DrawRectangle(locationX, locationY, width, height, inputColor);
};
Character::Character (const char * imageTexture, float speed) : characterHealth(300), speed(speed) {
            width = 896/56;
            height = 640/20;
            maxCols = 6;
            colIndex = startCols;
            rowIndex = 2;
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
                playerState = Hurt;
                isTakeDamage = true;
                return;
            }
            takeDamageTimeCap += deltaTime;
            takeDamageAnimationTime += deltaTime;
            if (takeDamageAnimationTime > 0.5) {
                isTakeDamage = false;
            }
            if (isTakeDamage) rowIndex = 19;
        }
void Character::takeDamage2 (Character* secondCollider, Vector2 MapPos, float deltaTime) {
            if (checkIsCollide(getCharacterHitBox(), secondCollider->getCharacterHitBox(), MapPos, 0, 0).isCollide && takeDamageTimeCap >= 1.5f && secondCollider->isAttacking) {
                characterHealth.takeDamage(100);
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
            if (playerState != Attacking) {
                if (playerState != Idle) {
                    switch (directionState)
                    {
                    case Right:
                        rowIndex = 2;
                        startCols = 0;
                        maxCols = 6;
                        break;
                    case Left:
                        rowIndex = 2;
                        startCols = 12;
                        maxCols = 18;
                        break;
                    case Up:
                        rowIndex = 2;
                        startCols = 6;
                        maxCols = 12;
                        break;
                    case Down:
                        rowIndex = 2;
                        startCols = 18;
                        maxCols = 24;
                        break;
                    default:
                        break;
                    }
                } else {
                    switch (directionState)
                    {
                    case Right:
                        rowIndex = 1;
                        startCols = 0;
                        maxCols = 6;
                        break;
                    case Left:
                        rowIndex = 1;
                        startCols = 12;
                        maxCols = 18;
                        break;
                    case Up:
                        rowIndex = 1;
                        startCols = 6;
                        maxCols = 12;
                        break;
                    case Down:
                        rowIndex = 1;
                        startCols = 18;
                        maxCols = 24;
                        break;
                    default:
                        break;
                    }
                }
            } else {
                switch (directionState)
                    {
                    case Right:
                        rowIndex = 14;
                        startCols = 0;
                        maxCols = 5;
                        break;
                    case Left:
                        rowIndex = 14;
                        startCols = 12;
                        maxCols = 18;
                        break;
                    case Up:
                        rowIndex = 14;
                        startCols = 6;
                        maxCols = 12;
                        break;
                    case Down:
                        rowIndex = 14;
                        startCols = 18;
                        maxCols = 24;
                        break;
                    default:
                        break;
                    }
            }
            if (isNeedResetCols) {
                isNeedResetCols = false;
                colIndex = startCols;
            }
            if (updateAnimationTime > 0.1) {
                updateAnimationTime = 0;
                colIndex++;
                if (colIndex > maxCols - 1) colIndex = startCols;
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
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
Vector2* Character::getWorldPosPointer () {
    return &worldPos;
}
void Character::updateDirectionState (Vector2 newDirection) {
    if (playerState != Hurt && playerState != Attacking) {
        if (newDirection.x >= 0) {
            playerState = Running;
            directionState = Right;
        };
        if (newDirection.x < 0) {
            playerState = Running;
            directionState = Left;
        };
        if (newDirection.y >= 0) {
            playerState = Running;
            if (newDirection.x < 0.2 && newDirection.x > -0.2) directionState = Down;
        };
        if (newDirection.y < 0) {
            playerState = Running;
            if (newDirection.x < 0.2 && newDirection.x > -0.2) directionState = Up;
        };
        if (newDirection.x == 0 && direction.y == 0) playerState = Idle;
        if (directionState != oldDirectionState) {
            isNeedResetCols = true;
            oldDirectionState = directionState;
        }
    }
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
HealthComponent Character::getHealthComponent () {
    return characterHealth;
}
Player::Player (const char * imageTexture, std::vector<std::vector<int>>* worldCollisionArray, float speed): Character(imageTexture, speed), worldCollisionArray(worldCollisionArray) {
            screenPos.x = SCREEN_WIDTH/2;
            screenPos.y = SCREEN_HEIGHT/2;
            worldPos = {200, 100};
            characterRecDes = {screenPos.x, screenPos.y, width*scale_factor, height*scale_factor};
            characterCollision = {screenPos.x, screenPos.y+12, width*scale_factor, (height-6)*scale_factor};
            characterHitBox = {screenPos.x + 30, screenPos.y, width*scale_factor, height*scale_factor};
        }
void Player::tick (float deltaTime) {
            if (!isTakeDamage) {
                if (direction.x == 0 && direction.y == 0) {
                }
                if (IsKeyPressed(KEY_K)) {
                    playerState = Attacking;
                    isAttacking = true;
                }
                if (IsKeyReleased(KEY_K)) {
                    isAttacking = false;
                    playerState = Idle;
                }
                if (playerState != Attacking) {
                    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S) ||
                        IsKeyReleased(KEY_A) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_D) || IsKeyReleased(KEY_S)) isNeedResetCols = true;
                    if (IsKeyDown(KEY_A)) {
                        directionState = Left;
                        direction.x = -1;
                    }
                    if (IsKeyDown(KEY_D)) {
                        directionState = Right;
                        direction.x = 1;
                    }
                    if (IsKeyDown(KEY_W)) {
                        if (direction.x == 0) directionState = Up;
                        direction.y = -1;
                    }
                    if (IsKeyDown(KEY_S)) {
                        if (direction.x == 0) directionState = Down;
                        direction.y = 1;
                    };
                    if (direction.x != 0 || direction.y != 0) rowIndex = 2;
                    isAttacking = false;
                }
                if (playerState != Attacking) {
                    if (direction.x != 0 || direction.y != 0) {
                        playerState = Walking;
                    } else {
                        playerState = Idle;
                    }
                }
                if(Vector2Length(direction) != 0) {
                    direction = Vector2Normalize(direction)*speed*deltaTime*50;
                    direction.x = (int)(std::round(direction.x)); 
                    direction.y = (int)(std::round(direction.y));
                    if (!checkPlayerCollisionTile(worldCollisionArray, characterCollision, worldPos, direction, 79732).isCollide) {
                        worldPos = Vector2Add(worldPos, direction);
                        }
                    }
                direction = {0,0};
            } 
                Character::tick(deltaTime);
            }
void Player::drawHealth(int x, int y) {
    characterHealth.drawHealth(x, y, characterHealth.currentHealth, 10, GREEN);
}
Vector2 Player::getWorldPos () {
            return worldPos;
        }
Vector2 Player::getScreenPos () {
            return screenPos;
        };
AIPlayer::AIPlayer (const char * imageTexture, Player* inputPlayer, int id, float speed): Character(imageTexture, speed), id(id) {
    player = inputPlayer;
}
void AIPlayer::AITick(float deltaTime, std::vector<AIPlayer>* allAIPlayer) {
    appraochTarget(allAIPlayer);
    Character::tick(deltaTime);
}
void AIPlayer::drawHealth() {
    characterHealth.healthDes = {characterCollision.x, characterCollision.y};
    characterHealth.drawHealth(characterHealth.healthDes.x, characterHealth.healthDes.y - 20, characterHealth.currentHealth/4, 10, RED);
}
void AIPlayer::doDamage() {
    playerState = Attacking;
    isAttacking = true;
}
void AIPlayer::appraochTarget (std::vector<AIPlayer>* allAIPlayer) {
    if (checkIsCollide(characterHitBox, player->getCharacterCollision(), {0,0}, 0, 0).isCollide) {
        doDamage();
    } else {
        playerState = Running;
        isAttacking = false;
    }
    if (!isNeedToMoveBack) {
        direction = Vector2Normalize(Vector2Subtract(player->getScreenPos(), screenPos));
        updateDirectionState(direction);
        for (AIPlayer &enemy : *allAIPlayer) {
            if (enemy.id != id) {
                CollisionProperty collisionProperty = checkIsCollide(characterCollision, enemy.getCharacterCollision(), {0,0}, 0, 0);
                if (collisionProperty.isCollide) {
                    if (characterCollision.x < collisionProperty.collider.x) direction.x = -1;
                    if (characterCollision.x >= collisionProperty.collider.x) direction.x = 1;
                    if (characterCollision.y < collisionProperty.collider.y) direction.y = -1;
                    if (characterCollision.y >= collisionProperty.collider.y) direction.y = 1;
                    direction = Vector2Normalize(direction);
                    isNeedToMoveBack = true;
                    this->collider = collisionProperty.collider;
                }
            }
            if (isNeedToMoveBack) break;
        }
    } else {
        if (std::abs(characterCollision.x - collider.x) > 50) {
            isNeedToMoveBack = false;
        }
    }
    if(!isTakeDamage) worldPos = Vector2Add(worldPos,direction*speed);
    setCharacterPos(worldPos, player->getWorldPos());
    }