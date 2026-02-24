#include "drawing.h"
#include <raylib.h>

Drawing::Drawing(float Y, int level) : Y(Y), drawLevel(level) {};

float Drawing::getY() {
    return Y;
}
void Drawing::setY(float Y) {
    this->Y = Y;
}
void Drawing::setDrawLevel(int level) {
    drawLevel = level;
}
int Drawing::getDrawLevel() {
    return drawLevel;
}
void Drawing::draw(Vector2 des) {};