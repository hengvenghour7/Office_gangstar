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
    startButton("Start Game", Vector2Add(location, {60 , 40*1}), MenuActionEnums::Start),
    saveButton("Save Game", Vector2Add(location, {60 , 40*2}), MenuActionEnums::Save),
    loadButton("Load Game", Vector2Add(location, {60 , 40*3}), MenuActionEnums::Load)
{
    Buttons.push_back(&startButton);
    Buttons.push_back(&saveButton);
    Buttons.push_back(&loadButton);
}
void UI::draw() {
    DrawTexturePro(BackgroundTexture, {0,0, 240, 320}, {location.x, location.y, 240, 320}, {0,0}, 0, WHITE);
    for (Button* &component : Buttons) {
        Rectangle button_dimension = component->getButtonRec();
        if (checkMouseOnHover(button_dimension).isCollide) {
            drawButtonBackground(button_dimension, GetColor(HOVER_BACKGROUND_COLOR), 10, 3);
        }
        component->draw();
    }
}
std::vector<Button*> UI::getMenuButton() {
    return Buttons;
}
ShopUI::ShopUI ():
    backgroundTexture(LoadTexture("resources/image/UI/shop/shop_background.png")),
    location({SCREEN_WIDTH/2 - 480*0.5, SCREEN_HEIGHT*0.5 - 320*0.5}) {
}
void ShopUI::setShopItems(std::vector<ShopItem>* shop_items) {
    shopItems = {};
    int column{0};
    int row{0};
    int temp_index{0};
    Rectangle tempDimension{location.x + 40 + 70*column, location.y + 50, 50, 50};
    for (ShopItem &item: *shop_items) {
        shopItems.emplace_back(&item.texture, item.name, item.heal, item.energyHeal, tempDimension, temp_index);
        column++;
        tempDimension.x = location.x + 40 + 70*column;
        temp_index++;
    };
};
void ShopUI::draw () {
    DrawTexturePro(backgroundTexture, {0,0, 480, 320}, {location.x, location.y, 480, 320 }, {0,0}, 0, WHITE);
        for (ShopUIItem &item: shopItems) {
            if (checkMouseOnHover(item.dimension).isCollide) {
                drawButtonBackground(item.dimension, GetColor(HOVER_BACKGROUND_COLOR), 5, 5);
            }
            item.draw();
        }
}
void ShopUI::handleInteraction(Player& player) {
    for (ShopUIItem &item: shopItems) {
        if (checkButtonClick(item.dimension).isCollide) {
            player.getPlayerInventory()->AddItems(item);
            std::cout<< (*player.getPlayerInventory()->getItems()).size();
        }
    }
};