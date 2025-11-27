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
        if (dataArray[tileY][tileX] == collisionCode) {
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
void MapBoundary::setCollisionData(std::vector<int>* mapCollisionData, int mapWidth, int mapHeight) {
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
            // if (dataArray[y][x] == collisionCode) {
            //     props.emplace_back("resources/image/Modern_UI_Style_1.png", x*16*1.5, y*16*1.5, 240/15, 688/21, 8, 10, 5);
            // };
            // if (dataArray[y][x] == 79740) {
            //     props.emplace_back("resources/image/Modern_UI_Style_1.png", x*16*1.5, y*16*1.5, 240/15, 688/21, 8, 10, 17);
            // }
            // if (dataArray[y][x] == 79730) {
            //     props.emplace_back("resources/image/Modern_UI_Style_1.png", x*16*1.5, y*16*1.5, 240/15, 64, 8, 10, 3);
            // }
            // if (dataArray[y][x] == 79735) {
            //     props.emplace_back("resources/image/Fishing_Boat_left.png", x*16*1.5, y*16*1.5, 768/8, 688/21, 8, 10, 3);
            // }
            // if (dataArray[y][x] == 79736) {
            //     props.emplace_back("resources/image/Fishing_Boat_left_2.png", x*16*1.5, y*16*1.5, 240/15, 688/21, 8, 10, 3);
            // }
        }
    };
}
void MapProp::drawAllProps (float scale, Vector2 mapPos, float deltaTime) {
    for (Prop &prop: props) {
        prop.drawProp(mapPos, deltaTime);
    }
}
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
Prop::Prop (const char* inputPropTexture, float inputX, float inputY, float inputPropWidth, float inputPropHeight, float inputCol, float inputRow, float inputMaxCol, float scale): scale(scale) {
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
    std::cout<< "ffff" << inputPropTexture << ", " << x << " " << y << " iiii";
};
void Prop::drawProp (Vector2 mapPos, float deltaTime) {
    updatePropTime += deltaTime;
    if (updatePropTime > 0.2) {
        if (initialCol > maxCol) initialCol = startCol;
        initialCol++;
        updatePropTime = 0;
    }
    DrawTexturePro(propTexture, {initialCol* propWidth,row* propHeight, propWidth, propHeight}, {x + mapPos.x,y + mapPos.y, propWidth*scale, propHeight*scale}, {0,0}, 0, WHITE);
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