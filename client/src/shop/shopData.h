#ifndef SHOPDATA_H
#define SHOPDATA_H

#include "../worldEnums.h"
#include <vector>
#include <iostream>
#include <unordered_map>

struct shopData
{
    // world:
};
struct ShopItemProperties {
    std::string name;
    const char* textureSrc;
    int heal;
    int energyHeal;
};
struct ShopProperties {
    std::string shopName;
    std::vector<ShopItemProperties> shopItems;
};
extern std::unordered_map<WorldEnums , std::vector<ShopProperties>> allShopItem;

#endif