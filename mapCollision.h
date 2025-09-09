#include <vector>
#include <iostream>
#include "helpers.h"


class MapBoundary {
    private:
    int inputCollisionSize{};
    std::vector<std::vector<int>> dataArray {};
    int mapWidth;
    int mapHeight;
    public: 
    MapBoundary (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize);
    int getWidth ();
    std::vector<std::vector<int>> getCollisionBoundary();
    void drawBoundary(float scale, Vector2 mapPos);
    CollisionProperty checkBoundaryCollision(Rectangle characterCollision, Vector2 mapPos, float XOffset, float YOffset);
};