#include "shop.h"
#include "../globalVar.h"

ShopItem::ShopItem (const char* textureSrc, std::string name, int heal, int energyHeal): texture(LoadTexture(textureSrc)),
    name(name), heal(heal), energyHeal(energyHeal) {

}

ShopUIItem::ShopUIItem(Texture2D* texture, std::string name, int heal, int energyHeal, Rectangle dimension):
    texture(texture), name(name), heal(heal), energyHeal(energyHeal),
    dimension(dimension) {
}
Rectangle ShopUIItem::getDimension() {
    return dimension;
}
void ShopUIItem::changeDimension(Rectangle dimension) {
    this->dimension = dimension;
};
void ShopUIItem::draw() {
    DrawTexturePro(*texture, {0,0, 16, 16}, dimension, {0,0}, 0, WHITE);
    DrawText(name.c_str(), dimension.x, dimension.y + 50, 16, WHITE);
}
Inventory::Inventory (std::vector<ShopItem> shopItems): backgroundTexture(LoadTexture("resources/image/UI/shop/shop_background.png")), 
    dimension {SCREEN_WIDTH/2 - 480*0.5, SCREEN_HEIGHT*0.5 - 320*0.5, 480, 320} {
    int column{0};
    Rectangle temp_dimension = {dimension.x + 40 + 70*column, dimension.y + 50, 50,50};
    for (ShopItem item: shopItems) {
        items.emplace_back(&item.texture, item.name, item.heal, item.energyHeal, temp_dimension);
        column++;
        temp_dimension.x = temp_dimension.x + 40 + 70*column;
    }
}
void Inventory::draw () {
    DrawTexturePro(backgroundTexture, {0,0, 480, 320}, {dimension.x, dimension.y, 480, 320 }, {0,0}, 0, WHITE);
    for (ShopUIItem item: items) {
        item.draw();
    }
}
void Inventory::AddItems(ShopUIItem item) {
    Rectangle temp_dimension {};
    if (items.size() > 0) {
        temp_dimension = items[items.size()-1].getDimension();
        temp_dimension.x = temp_dimension.x + 70;
    } else {
        temp_dimension = {SCREEN_WIDTH/2 - 480*0.5 + 70, SCREEN_HEIGHT*0.5 - 320*0.5 + 50, 50, 50};
    }
    item.changeDimension(temp_dimension);
    items.push_back(item);
};
std::vector<ShopUIItem>* Inventory::getItems() {
    return &items;
}