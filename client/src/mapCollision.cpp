#include <vector>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"

MapBoundary::MapBoundary(std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, int inputCollisionCode) {
    
    collisionCode = inputCollisionCode;
    for (int j=0 ; j < (int)inputDataArray.size(); j+= inputMapWidth) {
        std::vector<int> chunk{};
        for (int i = j; i< j + inputMapWidth; i++) {
            chunk.push_back(inputDataArray[i]);
        }
        dataArray.push_back(chunk);
    }
    mapWidth = inputMapWidth;
    mapHeight = inputMapHeight;
}
int MapBoundary::getWidth() {
    return mapWidth;
}
std::vector<std::vector<int>> MapBoundary::getCollisionBoundary() {
    return dataArray;
}
void MapBoundary::drawBoundary(float scale, Vector2 mapPos) {
    for (int y = 0; y < (int)dataArray.size(); y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (dataArray[y][x] == collisionCode) {
                DrawRectangle (x*16*scale + mapPos.x,y*16*scale + mapPos.y, 16*scale, 16*scale, GREEN);
            }
        }
    };
}
CollisionProperty MapBoundary::checkBoundaryCollision (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y + 8};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, worldPos);
    int tileX = (int)(playerWorldPos.x + XOffset)/16/1.5; // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)(playerWorldPos.y + YOffset)/16/1.5;
    if (tileY < dataArray.size() && tileX < mapWidth) {
        if (dataArray[tileY][tileX] == collisionCode || dataArray[tileY][tileX] == 79733) {
                collision1.isCollide = true;
                return collision1;
            }
    }
    return collision1;
}
CollisionProperty MapBoundary::checkInteractionBoundary (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset, int colorCode) {
    CollisionProperty collision1 {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y + 8};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, worldPos);
    int tileX = (int)(playerWorldPos.x + XOffset)/16/1.5; // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)(playerWorldPos.y + YOffset)/16/1.5;
    if (dataArray[tileY][tileX] == colorCode) {
            collision1.isCollide = true;
            return collision1;
        }
    return collision1;
}
void MapBoundary::setCollisionData(std::vector<int>* mapCollisionData, int mapWidth, int mapHeight, int collisionCode) {
    this->collisionCode = collisionCode;
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    dataArray = {};
    for (int j=0 ; j < (int)mapCollisionData->size(); j+= this->mapWidth) {
        std::vector<int> chunk{};
        for (int i = j; i< j + this->mapWidth; i++) {
            chunk.push_back((*mapCollisionData)[i]);
        }
        dataArray.push_back(chunk);
    }
    std::cout<< "this width" << this->mapWidth << "height" << this->mapHeight;
};
MapProp::MapProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition> PropDrawConditions, float scale) : MapBoundary (inputDataArray,inputMapWidth, inputMapHeight, inputTileSize, 99),
PropDrawConditions(PropDrawConditions), scale(scale)
 {
    const int MAP_TILE_SIZE(16);
    for (int y = 0; y < (int)dataArray.size(); y++) {
        for (int x = 0; x < mapWidth; x++) {
            for (PropDrawCondition condition : this->PropDrawConditions) {
                if (dataArray[y][x] == condition.collisionCode) {
                    props.emplace_back(condition.imagePath, x*MAP_TILE_SIZE*this->scale, y*MAP_TILE_SIZE*this->scale, condition.width, condition.height, condition.startCol, condition.startRow, condition.maxCols, scale);
                }
            }
        }
    };
}
void MapProp::moveProps (float speedX, float speedY) {
    for (Prop &prop : props) {
        prop.x += speedX;
    }
}
void MapProp::moveProps2 (float speedX, float speedY, int colorCode) {
    for (Prop &prop : props) {
        checkPath(&dataArray, {prop.x, prop.y}, &prop.direction, colorCode);
        prop.x += prop.direction.x;
        prop.y += prop.direction.y;
    }
}
void MapProp::drawAllProps (float scale, Vector2 mapPos, float deltaTime, bool isBackward, bool isPauseisPauseAfterAnimated) {
    for (Prop &prop: props) {
        prop.drawProp(mapPos, deltaTime, isBackward, isPauseisPauseAfterAnimated);
    }
};
CollisionProperty MapProp::checkInteraction (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y + 8};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, worldPos);
    int tileX = (int)(playerWorldPos.x + XOffset)/16/1.5; // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)(playerWorldPos.y + YOffset)/16/1.5;
    if (dataArray[tileY][tileX] == 79742) {
            collision1.isCollide = true;
            // std::cout<< "color collll";
            return collision1;
        }
    return collision1;
}
Prop::Prop (const char* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputRow, float inputMaxCol, float scale): scale(scale), isFirstAction(true) {
    // std::cout<<"ttt" << inputPropTexture;
    startCol = inputCol;
    initialCol = inputCol;
    maxCol = inputMaxCol;
    propTexture = LoadTexture(inputPropTexture);
    row = inputRow;
    x = inputX;
    y = inputY;
    propWidth = inputPropWidth;
    propHeight = inputPropHeight;
};
void Prop::drawProp (Vector2 mapPos, float deltaTime, bool isBackward, bool isPauseisPauseAfterAnimated) {
    updatePropTime += deltaTime;
    if (updatePropTime > 0.2) {
        if (isBackward) {
            if (!(isPauseisPauseAfterAnimated && !isFirstAction)) {
                if (initialCol < startCol) {
                    initialCol = maxCol;
                    if (isPauseisPauseAfterAnimated) isFirstAction = false;
                };
                initialCol--;
                updatePropTime = 0;
            };
        } else {
                // if (isPauseisPauseAfterAnimated && initialCol == startCol) return;
                if (initialCol > maxCol) initialCol = startCol;
                initialCol++;
                updatePropTime = 0;
        }
    }
    DrawTexturePro(propTexture, {initialCol* propWidth,row* propHeight, propWidth, propHeight}, {x + mapPos.x,y + mapPos.y, propWidth*scale, propHeight*scale}, {0,0}, 0, WHITE);
}
void Prop::setIsFirstAction(bool isFirstAction) {
    this->isFirstAction = isFirstAction;
}
MapHandler::MapHandler (Vector2* inputMapPos, float inputScale, std::vector<int>* mapCollisionData) {
    drawTexture = outsideMap;
    mapPos = inputMapPos;
    scale = inputScale;
    this->mapCollisionData = mapCollisionData;
}
void MapHandler::drawMap (Vector2 mapPos) {
    DrawTextureEx(drawTexture, mapPos, 0,scale,WHITE);
}
void MapHandler::changeMap (Map inputMap) {
    drawTexture = inputMap.mapTexture;
}
InteractableProp::InteractableProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition> propCollisionConditions, float scale)
: MapProp (inputDataArray, inputMapWidth, inputMapHeight, inputTileSize, propCollisionConditions, scale), isOn(false) {};
void InteractableProp::toggleDraw(float scale, Vector2 mapPos, float deltaTime) {
    if (!isOn) {
        drawAllProps(scale, mapPos, deltaTime, true, true);
    } else {
        drawAllProps(scale, mapPos, deltaTime, true, false);
    }
}
void InteractableProp::toggleIsOn() {
    isOn = !isOn;
}
void checkPath (std::vector<std::vector<int>>* tileArray, Vector2 actorPos, Vector2* direction, int colorCode) {
    int tileX = (int)((actorPos.x)/16/1.5); // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)((actorPos.y)/16/1.5);
    // std::cout<<"y"<< (*tileArray)[tileY+1][tileX + 2];
    Vector2 nextDirection{0,0};
    if (direction->x >= 0 && tileX >= 0 && tileY >= 0) {
        if ((*tileArray)[tileY][tileX + 1] == colorCode) {
            direction->x = 1;
            direction->y = 0;
            return;
        }
        if (direction->y <= 0) {
            if ((*tileArray)[tileY-1][tileX] == colorCode) {
                direction->y = -1;
                direction->x = 0;
                return;
            }
            if ((*tileArray)[tileY + 2][tileX] == colorCode) {
                direction->y = 1;
                direction->x = 0;
                return;
            } 
        }
        if (direction->y > 0) {
            if ((*tileArray)[tileY+1][tileX] == colorCode) {
                direction->y = 1;
                direction->x = 0;
                return;
            }
            if ((*tileArray)[tileY][tileX - 1] == colorCode) {
            direction->x = -1;
            std::cout<< "this line hit";
            direction->y = 0;
            return;
            }
            if ((*tileArray)[tileY-1][tileX] == colorCode) {
                direction->y = -1;
                direction->x = 0;
                return;
            }
        }
        
    }
    if (direction->x < 0 && tileX >= 0 && tileY >= 0) {
        if ((*tileArray)[tileY][tileX - 1] == colorCode && tileX - 1 >= 0) {
            direction->x = -1;
            direction->y = 0;
            return;
        }
        direction->x = 1;
        direction->y = 0;
    }
}