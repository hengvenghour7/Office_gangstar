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
ShopItem::ShopItem (const char* textureSrc, std::string name, int heal, int energyHeal): texture(LoadTexture(textureSrc)),
    name(name), heal(heal), energyHeal(energyHeal) {

}
ShopUI::ShopUI ():
    backgroundTexture(LoadTexture("resources/image/UI/shop/shop_background.png")),
    location({SCREEN_WIDTH/2 - 480*0.5, SCREEN_HEIGHT*0.5 - 320*0.5}) {

}
void ShopUI::setShopItems(std::vector<ShopItem>* shop_items) {
    shopItems = shop_items;
};
void ShopUI::draw () {
    DrawTexturePro(backgroundTexture, {0,0, 480, 320}, {location.x, location.y, 480, 320 }, {0,0}, 0, WHITE);
    if (shopItems != nullptr) {
        // std::cout<<"ffffvv " << (*shopItems).size();
        Vector2 itemPosition {};
        for (ShopItem &item: *shopItems) {
            DrawTexturePro(item.texture, {0,0, 16, 16}, {location.x + 50 + 50*itemPosition.x, location.y + 50, 50, 50 }, {0,0}, 0, WHITE);
            DrawText(item.name.c_str(), location.x + 60 + 50*itemPosition.x, location.y + 60 + 32, 16, WHITE);
            itemPosition.x++;
        }
    }
}