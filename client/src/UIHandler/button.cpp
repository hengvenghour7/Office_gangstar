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