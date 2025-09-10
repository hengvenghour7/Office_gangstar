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
    if (dataArray[tileY][tileX] == 79732) {
            collision1.isCollide = true;
            return collision1;
        }
    return collision1;
}
MapProp::MapProp (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize, int inputCollisionCode) : MapBoundary (inputDataArray,inputMapWidth, inputMapHeight, inputTileSize, inputCollisionCode)
 {
    for (int y = 0; y < (int)dataArray.size(); y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (dataArray[y][x] == collisionCode) {
                props.emplace_back("resources/image/Modern_UI_Style_1.png", x*16*1.5, y*16*1.5, 240/15, 688/21);
            };
        }
    };
}
void MapProp::drawAllProps (float scale, Vector2 mapPos, float deltaTime) {
    for (Prop &prop: props) {
        prop.drawProp(mapPos, deltaTime);
    }
}
Prop::Prop (const char* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight) {
    // std::cout<<"ttt" << inputPropTexture;
    propTexture = LoadTexture(inputPropTexture);
    x = inputX;
    y = inputY;
    propWidth = inputPropWidth;
    propHeight = inputPropHeight;
};
void Prop::drawProp (Vector2 mapPos, float deltaTime) {
    updatePropTime += deltaTime;
    if (updatePropTime > 0.2) {
        if (initialCol > maxCol) initialCol = 8;
        initialCol++;
        updatePropTime = 0;
    }
    // std::cout << "innn"<< initialCol << "uuubbb" << x;
    DrawTexturePro(propTexture, {initialCol* 240/15,5* 688/21, 240/15, 688/21}, {x + mapPos.x,y + mapPos.y, 240/15*1.5, 688/21*1.5}, {0,0}, 0, WHITE);
}