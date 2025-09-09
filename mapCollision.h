#include <vector>
#include <iostream>

struct Boundary {
    int x;
    int y;
    float width;
    float height;
};
struct CollisionProperty {
    bool isCollide;
    Boundary collider;
};
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
    CollisionProperty checkBoundaryCollision(Boundary characterCollision, Vector2 mapPos, float XOffset, float YOffset);
};

CollisionProperty checkIsCollide (Boundary firstCollider, Boundary secondCollider, Vector2 mapPos, float XOffset, float YOffset);
extern MapBoundary mapCollision;