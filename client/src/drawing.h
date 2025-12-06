#ifndef DRAWING_H
#define DRAWING_H
#include <raylib.h>

class Drawing {
    protected:
    float Y{};
    
    public:
    Drawing();
    float getY();
    void setY(float Y);
    virtual void draw(Vector2 des);
};

#endif