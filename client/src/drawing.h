#ifndef DRAWING_H
#define DRAWING_H
#include <raylib.h>

class Drawing {
    protected:
    float Y{};
    int drawLevel{};
    
    public:
    Drawing(float Y = 0, int level = 0);
    float getY();
    void setY(float Y);
    void setDrawLevel(int level);
    int getDrawLevel();
    virtual void draw(Vector2 des);
};

#endif