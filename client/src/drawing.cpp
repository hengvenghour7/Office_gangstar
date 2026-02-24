#include "drawing.h"
#include <raylib.h>

Drawing::Drawing() : Y(0) {};

float Drawing::getY() {
    return Y;
}
void Drawing::setY(float Y) {
    this->Y = Y;
}
void Drawing::setDrawLevel(int level) {
    drawLevel = level;
}
void Drawing::draw(Vector2 des) {};