#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include "../drawing.h"
#include <iostream>


class InteractableItem: public Drawing {
    Texture2D itemTexture;
    int textureWidth;
    int textureHeight;
    Rectangle dimension;
    public:
        InteractableItem(int width, int height,const char* textureSrc, Rectangle dimension);
        Texture2D* getTexture();
        Rectangle getDimension();
        Vector2 getCenter(Vector2 mapPos);
        virtual void draw(Vector2 mapPos) override;
        void changeDimension(Rectangle dimension);
        void changeLocation(Vector2 location);
        int getTextureWidth();
        int getTextureHeight();
};

#endif