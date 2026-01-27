#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include "../drawing.h"
#include <iostream>


class InteractableItem: public Drawing {
    Texture2D itemTexture;
    Rectangle dimension;
    public:
        InteractableItem(const char* textureSrc, Rectangle dimension);
        Texture2D* getTexture();
        Rectangle getDimension();
        Vector2 getCenter(Vector2 mapPos);
        virtual void draw(Vector2 mapPos) override;
};

#endif