#include "../globalVar.h"
#include "item.h"

InteractableItem::InteractableItem (int width, int height, const char* textureSrc, Rectangle dimension): itemTexture(LoadTexture(textureSrc)),
    dimension(dimension), textureWidth(width), textureHeight(height) {
}
int InteractableItem::getTextureWidth() {
    return textureWidth;
}
int InteractableItem::getTextureHeight() {
    return textureHeight;
}
void InteractableItem::draw(Vector2 mapPos) {
    DrawTexturePro(itemTexture, {0 , 0, (float)textureWidth, (float)textureHeight}, {dimension.x + mapPos.x,dimension.y + mapPos.y, dimension.width, dimension.height}, {0,0}, 0, WHITE);
}
Texture2D* InteractableItem::getTexture() {
    return &itemTexture;
}
Rectangle InteractableItem::getDimension() {
    return dimension;
}
Vector2 InteractableItem::getCenter(Vector2 mapPos) {
    Vector2 center{dimension.x + dimension.width*0.5f + mapPos.x, 
        dimension.y + dimension.height*0.5f + mapPos.y};
    return center;
}
void InteractableItem::changeDimension(Rectangle dimension) {
    this->dimension = dimension;
}
void InteractableItem::changeLocation(Vector2 location) {
    setY(3000);
    this->dimension = {location.x, location.y, this->dimension.width, this->dimension.height};
}