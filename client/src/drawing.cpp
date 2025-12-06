#include "drawing.h"

Drawing::Drawing() : Y(0) {};

float Drawing::getY() {
    return Y;
}
void Drawing::setY(float Y) {
    this->Y = Y;
}

void Drawing::draw() {};