#include <vector>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "mapCollision.h"

MapBoundary::MapBoundary(std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize) {
    
    int n = 0;
    inputCollisionSize = inputDataArray.size();
    for (int j=0 ; j < (int)inputDataArray.size(); j+= inputMapWidth) {
        std::vector<int> chunk{};
        for (int i = j; i< j + inputMapWidth; i++) {
            chunk.push_back(inputDataArray[i]);
            // if (inputDataArray[i] == 79732) {
            //     dataArray.push_back({i-j, n, 16, 16});
            // }
        }
        n++;
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
    Texture2D arrowImg = LoadTextureFromImage(LoadImage("resources/image/Modern_UI_Style_1.png"));
    for (int y = 0; y < (int)dataArray.size(); y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (dataArray[y][x] == 79732) {
                // DrawRectangle (x*16*scale + mapPos.x,y*16*scale + mapPos.y, 16*scale, 16*scale, GREEN);
                DrawTexturePro(arrowImg, {6* 240/15,5* 688/21, 240/15, 688/21}, {x*16*scale + mapPos.x,y*16*scale + mapPos.y, 240/15*scale, 688/21*scale}, {0,0}, 0, WHITE);
            }
        }
    };
}
CollisionProperty MapBoundary::checkBoundaryCollision (Rectangle characterCollision, Vector2 mapPos, float XOffset, float YOffset) {
    CollisionProperty collision1 {false, {}};
    Vector2 charCollisionScreenPos{XOffset > 0 ?  characterCollision.x + characterCollision.width : characterCollision.x,YOffset > 0 ? characterCollision.y + characterCollision.height : characterCollision.y};
    Vector2 playerWorldPos = Vector2Add(charCollisionScreenPos, mapPos);
    int tileX = (int)(playerWorldPos.x + XOffset)/16/1.5; // x*16*1.5 + mapPos.x -- -mapPos.x/16/1.5
    int tileY = (int)(playerWorldPos.y + YOffset)/16/1.5;
    if (dataArray[tileY][tileX] == 79732) {
            collision1.isCollide = true;
            return collision1;
        }
    return collision1;
}