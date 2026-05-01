#include "button.h"

BaseButton::BaseButton (ButtonParameter buttonDetail, Rectangle dimension, int index):
    name(buttonDetail.name),
    dimension(dimension),
    index(index),
    action(buttonDetail.action)
{

}
void BaseButton::doAction() {
    action();
}
Rectangle& BaseButton::getDimension() {
    return dimension;
}
int BaseButton::getIndex() {
    return index;
}
void BaseButton::draw()
{
}
void BaseButton::updatePosition(Vector2 pos) {
    dimension.x = pos.x;
    dimension.y = pos.y;
}
std::string& BaseButton::getName()
{
    return name;
}
CategoryTabButton::CategoryTabButton (Texture2D& texture, Rectangle srcDimension, ButtonParameter buttonDetail, Rectangle dimension, int index)
: BaseButton(buttonDetail, dimension, index),
srcDimension(srcDimension),
texture(texture)
{

}
void CategoryTabButton::setIsTrigger(bool isTrigger)
{
    this->isTrigger = isTrigger;
}
bool CategoryTabButton::getIsTrigger()
{
    return isTrigger;
}
void CategoryTabButton::draw()
{
    DrawTexturePro(texture, srcDimension, dimension, {0, 0}, 0, WHITE);
}