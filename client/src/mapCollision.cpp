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
};
MapProp::MapProp (std::vector<int>* inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition>* propCollisionConditions, float scale) : locationArray(arrayTo2DArray(inputDataArray, inputMapWidth)), PropDrawConditions(propCollisionConditions), scale(scale)
 {
    const int MAP_TILE_SIZE(16);
    for (int y = 0; y < (int)locationArray.size(); y++) {
        for (int x = 0; x < inputMapWidth; x++) {
            for (PropDrawCondition &condition : *PropDrawConditions) {
                if (locationArray[y][x] == condition.collisionCode) {
                    props.emplace_back(&condition.imagePath, x*MAP_TILE_SIZE*this->scale, y*MAP_TILE_SIZE*this->scale, condition.width, condition.height, condition.startCol, condition.startRow, condition.maxCols, scale);
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
        prop.x += prop.direction.x;
        prop.y += prop.direction.y;
    }
}
void MapProp::draw (Vector2 mapPos) {
    for (Prop &prop: props) {
        prop.draw(mapPos);
    }
};
void MapProp::updateAnimation(float deltaTime, bool isBackward, bool isPauseisPauseAfterAnimated) {
    for (Prop &prop: props) {
        prop.updateAnimation(deltaTime, isBackward, isPauseisPauseAfterAnimated);
    }
};
CollisionProperty MapProp::checkInteraction (Rectangle characterCollision, Vector2 worldPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y + 8};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, worldPos);
    int tileX = (int)(playerWorldPos.x + XOffset)/16/1.5; // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)(playerWorldPos.y + YOffset)/16/1.5;
    if (locationArray[tileY][tileX] == 79742) {
            collision1.isCollide = true;
            return collision1;
        }
    return collision1;
}
Prop::Prop (Texture2D* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputRow, float inputMaxCol, float scale): scale(scale), isFirstAction(true), propTexture(inputPropTexture) {
    startCol = inputCol;
    initialCol = inputCol;
    maxCol = inputMaxCol;
    row = inputRow;
    x = inputX;
    y = inputY;
    propWidth = inputPropWidth;
    propHeight = inputPropHeight;
};
void Prop::draw (Vector2 mapPos) {
    DrawTexturePro(*propTexture, {initialCol* propWidth,row* propHeight, propWidth, propHeight}, {x + mapPos.x,y + mapPos.y, propWidth*scale, propHeight*scale}, {0,0}, 0, WHITE);
}
void Prop::updateAnimation(float deltaTime, bool isBackward, bool isPauseisPauseAfterAnimated) {
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
                if (initialCol > maxCol) initialCol = startCol;
                initialCol++;
                updatePropTime = 0;
        }
    }
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
InteractableProp::InteractableProp (std::vector<int>* inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition>* propCollisionConditions, float scale)
: MapProp (inputDataArray, inputMapWidth, inputMapHeight, inputTileSize, propCollisionConditions, scale), isOn(false) {};
void InteractableProp::toggleDraw(float scale, Vector2 mapPos, float deltaTime) {
    // if (!isOn) {
    //     drawAllProps(scale, mapPos, deltaTime, true, true);
    // } else {
    //     drawAllProps(scale, mapPos, deltaTime, true, false);
    // }
}
void InteractableProp::toggleIsOn() {
    isOn = !isOn;
}