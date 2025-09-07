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
    std::vector<Boundary> dataArray {};
    int mapWidth;
    int mapHeight;
    public: 
    MapBoundary (std::vector<int> inputDataArray, int inputMapWidth, int inputMapHeight, int inputTileSize);
    int getWidth ();
    std::vector<Boundary> getCollisionBoundary();
    void drawBoundary(float scale, Vector2 mapPos);
    CollisionProperty checkBoundaryCollision(Boundary characterCollision, Vector2 mapPos);
};

CollisionProperty checkIsCollide (Boundary firstCollider, Boundary secondCollider, Vector2 mapPos);
extern MapBoundary mapCollision;