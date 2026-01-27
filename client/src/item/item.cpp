#include "item.h"

InteractableItem::InteractableItem (const char* textureSrc, Rectangle dimension): itemTexture(LoadTexture(textureSrc)),
    dimension(dimension) {
}
void InteractableItem::draw(Vector2 mapPos) {
    DrawTexturePro(itemTexture, {0 , 0, 32, 16}, {dimension.x + mapPos.x,dimension.y + mapPos.y, dimension.width, dimension.height}, {0,0}, 0, WHITE);
}
Texture2D* InteractableItem::getTexture() {
    return &itemTexture;
}
Rectangle InteractableItem::getDimension() {
    return dimension;
}