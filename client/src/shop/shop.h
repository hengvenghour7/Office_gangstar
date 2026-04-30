#ifndef SHOP_H
#define SHOP_H

#include <raylib.h>
#include <iostream>
#include <vector>
#include <../UIHandler/button.h>

enum class CategoryState {
    Inventory,
    Status,
    Skill,
    Weapon,
    Setting
};
class InventoryCategory: public BaseButton {
    Texture2D itemTexture;
    float srcWidth;
    float srcHeight;
    public:
        InventoryCategory(ButtonParameter buttonDetail, Rectangle dimension, int index);
        virtual void draw() override;
};
class ShopItem {
    public:
        Texture2D texture;
        std::string name;
        int heal;
        int energyHeal;
        Rectangle dimension;
        int price;
        std::string description;
        ShopItem(const char* textureSrc, std::string name, int heal, int energyHeal, int price, std::string description);
};
class ShopUIItem {
    public:
        Texture2D* texture;
        std::string name;
        int index;
        int heal;
        int energyHeal;
        int price;
        std::string description;
        Rectangle dimension;
        ShopUIItem(Texture2D* texture, std::string name, int heal, int energyHeal, Rectangle dimension, int index, int price, std::string description);
        Rectangle getDimension();
        int getIndex();
        void changeDimension(Rectangle dimension);
        void changeIndex (int index);
        void draw();
};

class Inventory {
    std::vector<ShopUIItem> items{};
    Rectangle dimension;
    Texture2D backgroundTexture;
    Texture2D categoryBackground;
    Texture2D iconTextureSet;
    std::unordered_map<std::string, InventoryCategory> allCategories;
    int categoryWidth;
    int categoryHeight;
    std::string currentCategory;
    CategoryState categoryState;
    float categoryButtonYOffset;
    public:
        Inventory(std::vector<ShopItem> shopItems);
        void tick();
        void handleCategoryClick();
        void AddItems(ShopUIItem item);
        std::vector<ShopUIItem>* getItems();
        void reArrangeItems();
        CategoryState& getCategoryState();
        void draw();
        void drawInventory();
        void drawSkill();
        void drawStatus();
        void drawSetting();
        void drawWeapon();
        void updateCategoryButtonPos(std::string targetCategory);
};

#endif