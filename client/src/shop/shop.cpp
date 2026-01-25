#include "shop.h"
#include "../globalVar.h"
#include "../helpers.h"

ShopItem::ShopItem (const char* textureSrc, std::string name, int heal, int energyHeal): texture(LoadTexture(textureSrc)),
    name(name), heal(heal), energyHeal(energyHeal) {

}

ShopUIItem::ShopUIItem(Texture2D* texture, std::string name, int heal, int energyHeal, Rectangle dimension, int index):
    texture(texture), name(name), heal(heal), energyHeal(energyHeal),
    dimension(dimension), index(index) {
}
Rectangle ShopUIItem::getDimension() {
    return dimension;
}
int ShopUIItem::getIndex() {
    return index;
}
void ShopUIItem::changeDimension(Rectangle dimension) {
    this->dimension = dimension;
};
void ShopUIItem::changeIndex(int index) {
    this->index = index;
}
void ShopUIItem::draw() {
    DrawTexturePro(*texture, {0,0, 16, 16}, dimension, {0,0}, 0, WHITE);
    DrawText(name.c_str(), dimension.x, dimension.y + 50, 16, WHITE);
}
Inventory::Inventory (std::vector<ShopItem> shopItems): backgroundTexture(LoadTexture("resources/image/UI/shop/shop_background.png")), 
    dimension {SCREEN_WIDTH/2 - 480*0.5, SCREEN_HEIGHT*0.5 - 320*0.5, 480, 320} {
    int column{0};
    int temp_index{0};
    Rectangle temp_dimension = {dimension.x + 40 + 70*column, dimension.y + 50, 50,50};
    for (ShopItem item: shopItems) {
        items.emplace_back(&item.texture, item.name, item.heal, item.energyHeal, temp_dimension, temp_index);
        column++;
        temp_dimension.x = temp_dimension.x + 40 + 70*column;
        temp_index++;
    }
}
void Inventory::draw () {
    DrawTexturePro(backgroundTexture, {0,0, 480, 320}, {dimension.x, dimension.y, 480, 320 }, {0,0}, 0, WHITE);
    for (ShopUIItem item: items) {
        Rectangle button_dimension = item.getDimension();
        if (checkMouseOnHover(button_dimension).isCollide) {
            drawButtonBackground(button_dimension, GetColor(HOVER_BACKGROUND_COLOR), 10, 3);
        }
        item.draw();
    }
}

void Inventory::AddItems(ShopUIItem item) {
    Rectangle temp_dimension {};
    int temp_index {};
    if (items.size() > 0) {
        temp_dimension = items[items.size()-1].getDimension();
        temp_index = items[items.size()-1].getIndex() + 1;
        temp_dimension.x = temp_dimension.x + 70;
        if (temp_dimension.x + temp_dimension.width  > dimension.x + dimension.width) {
            temp_dimension = {SCREEN_WIDTH/2 - 480*0.5 + 50, temp_dimension.y + 60, 50, 50};
        }
    } else {
        temp_dimension = {SCREEN_WIDTH/2 - 480*0.5 + 50, SCREEN_HEIGHT*0.5 - 320*0.5 + 50, 50, 50};
    }
    item.changeDimension(temp_dimension);
    item.changeIndex(temp_index);
    items.push_back(item);
};
void Inventory::reArrangeItems() {
    Rectangle temp_dimension {};
    for (int i = 0 ; i < items.size(); i++) {
        if (i == 0) {
            temp_dimension = {SCREEN_WIDTH/2 - 480*0.5 + 50, SCREEN_HEIGHT*0.5 - 320*0.5 + 50, 50, 50};
        } else {
            temp_dimension = items[i-1].getDimension();
            temp_dimension.x = temp_dimension.x + 70;
            if (temp_dimension.x + temp_dimension.width  > dimension.x + dimension.width) {
                temp_dimension = {SCREEN_WIDTH/2 - 480*0.5 + 50, temp_dimension.y + 60, 50, 50};
            }
        }
        items[i].changeDimension(temp_dimension);
        
    }
}
std::vector<ShopUIItem>* Inventory::getItems() {
    return &items;
}