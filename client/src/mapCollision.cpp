#include <vector>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"
#include "globalVar.h"

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
MapProp::MapProp (std::vector<int>* inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, std::vector<PropDrawCondition>* propCollisionConditions, float scale) : locationArray(arrayTo2DArray(inputDataArray, inputMapWidth)), 
    PropDrawConditions(propCollisionConditions), scale(scale)
 {
    const int MAP_TILE_SIZE(16);
    for (int y = 0; y < (int)locationArray.size(); y++) {
        for (int x = 0; x < inputMapWidth; x++) {
            for (PropDrawCondition &condition : *PropDrawConditions) {
                if (locationArray[y][x] == condition.collisionCode) {
                    props.emplace_back(&condition.imagePath, x*MAP_TILE_SIZE*this->scale, y*MAP_TILE_SIZE*this->scale, condition.width, condition.height, condition.startCol, condition.startRow, condition.maxCols, scale, condition.collisionCode, condition.followPathDetail);
                }
            }
        }
    };
    for (Prop &prop : props) {
        if (prop.followPathDetail.isFollowPath) {
            followPathProps.push_back(&prop);
        }
    }
}
void MapProp::moveProps (float speedX, float speedY) {
    for (Prop &prop : props) {
        prop.x += speedX;
    }
}
void MapProp::moveProps2 (float speedX, float speedY) {
    for (Prop* &prop : followPathProps) {
        findPath(&locationArray, {prop->x, prop->y}, &prop->direction, prop->followPathDetail.pathCode, prop->locationCode);
        prop->x += prop->direction.x*speedX;
        prop->y += prop->direction.y*speedX;
    }
}
void MapProp::draw (Vector2 mapPos) {
    for (Prop &prop: props) {
        prop.draw(mapPos);
    }
};
std::vector<Prop>* MapProp::getMapProp() {
    return &props;
}
void MapProp::updateAnimation(float deltaTime, bool isBackward, bool isPauseisPauseAfterAnimated) {
    for (Prop &prop: props) {
        prop.updateAnimation(deltaTime, isBackward, isPauseisPauseAfterAnimated);
    }
    moveProps2(1,1);
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
Prop::Prop (Texture2D* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputRow, float inputMaxCol, float scale, int locationCode, FollowPathDetail followPathDetail): 
    scale(scale), isFirstAction(true), propTexture(inputPropTexture), locationCode(locationCode),followPathDetail(followPathDetail) {
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
Vector2 Prop::getCenter (Vector2 mapPos) {
    Vector2 center = {x + mapPos.x - (propWidth*scale*0.5f),y + mapPos.y - (propHeight*scale*0.5f)};
    return center;
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
void Prop::displayerInteractionText(Vector2* speechLocation, Texture2D* speechBackground) {
    DrawTexturePro(* speechBackground, {0,0, 320, 96}, {speechLocation->x, speechLocation->y, 320, 96}, {0,0}, 0, WHITE);
    DrawText(interactionSpeech, speechLocation->x + 50, speechLocation->y + 50, 16, GetColor(THEMECOLOR));
}
const char* Prop::getInteractionSpeech () {
    const char* speech = interactionSpeechs[speechIndex];
    speechIndex++;
    if (speechIndex >= interactionSpeechs.size()) speechIndex = 0;
    return speech;
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