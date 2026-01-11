#include "../globalVar.h"
#include "UI.h"

Button::Button (const char* text, Vector2 location, MenuActionEnums action): text(text), location(location),
    action(action), dimension({(float)MeasureText(text, 20), 30.f}) {

}
Rectangle Button::getButtonRec () {
    return {location.x, location.y, dimension.x, dimension.y};
}
MenuActionEnums Button::getAction () {
    return action;
}
void Button::draw () {
    // DrawRectangle (location.x, location.y, dimension.x, dimension.y, RED);
    DrawText(text, location.x, location.y , 20, textColor);
}
UI::UI(): BackgroundTexture(LoadTexture("resources/image/UI/start_screen_UI.png")), 
    location{SCREEN_WIDTH*0.5 - 240*0.5f, SCREEN_HEIGHT*0.5 - 320*0.5},
    startButton("Start Game", Vector2Add(location, {40 , 30*1}), MenuActionEnums::Start),
    saveButton("Save Game", Vector2Add(location, {40 , 30*2}), MenuActionEnums::Save),
    loadButton("Load Game", Vector2Add(location, {40 , 30*3}), MenuActionEnums::Load)
{
    Buttons.push_back(&startButton);
    Buttons.push_back(&saveButton);
    Buttons.push_back(&loadButton);
}
void UI::draw() {
    DrawTexturePro(BackgroundTexture, {0,0, 240, 320}, {location.x, location.y, 240, 320}, {0,0}, 0, WHITE);
    for (Button* &component : Buttons) {
        component->draw();
    }
}
std::vector<Button*> UI::getMenuButton() {
    return Buttons;
}