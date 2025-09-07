#include <vector>
#include <iostream>

struct Boundary {
    public: 
    int x;
    int y;
};
class MapBoundary {
    private:
    std::vector<Boundary> dataArray {};
    int mapWidth;
    int mapHeight;
    public: 
    MapBoundary (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize);
    int getWidth ();
    std::vector<Boundary> getCollisionBoundary();
    void drawBoundary(float scale, Vector2 mapPos);
};

extern MapBoundary mapCollision;