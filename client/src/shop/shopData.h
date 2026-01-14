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
using AllShopProperties = std::unordered_map<std::string ,std::vector<ShopItemProperties>>;
extern std::unordered_map<WorldEnums , AllShopProperties> allShopItems;

class HandleWorldShop {
    public:
        HandleWorldShop();
        std::vector<ShopItemProperties> getShopItems();
};

#endif