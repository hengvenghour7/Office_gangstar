#ifndef SHOP_H
#define SHOP_H

#include <raylib.h>
#include <iostream>
#include <vector>

class ShopItem {
    public:
        Texture2D texture;
        std::string name;
        int heal;
        int energyHeal;
        Rectangle dimension;
        ShopItem(const char* textureSrc, std::string name, int heal, int energyHeal);
};
class ShopUIItem {
    public:
        Texture2D* texture;
        std::string name;
        int heal;
        int energyHeal;
        Rectangle dimension;
        ShopUIItem(Texture2D* texture, std::string name, int heal, int energyHeal, Rectangle dimension);
        Rectangle getDimension();
        void changeDimension(Rectangle dimension);
        void draw();
};
class Inventory {
    std::vector<ShopUIItem> items{};
    Rectangle dimension;
    Texture2D backgroundTexture;
    public:
        Inventory(std::vector<ShopItem> shopItems);
        void AddItems(ShopUIItem item);
        std::vector<ShopUIItem>* getItems();
        void draw();
};

#endif