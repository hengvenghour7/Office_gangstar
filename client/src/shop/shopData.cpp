#include "shopData.h"

std::unordered_map<WorldEnums , std::vector<ShopProperties>> allShopItem = {
    {
        WorldEnums::CenterWorld,
        {
            {
                "shop1",
                {
                    {
                        "meat",
                        "resources/image/UI/shop/meat_icon.png",
                        20,
                        10
                    }
                }
            },
        }
    }
};