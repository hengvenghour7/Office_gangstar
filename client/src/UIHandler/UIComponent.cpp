#include "UIComponent.h"

UIComponent::UIComponent(Rectangle dimension, std::string title, std::function<void()> action): dimension(dimension), title(title),
    action(action) {

}
void UIComponent::callAction () {
    action();
}
void UIComponent::draw() {

}