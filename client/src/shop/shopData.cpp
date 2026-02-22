#include "shopData.h"

std::unordered_map<WorldEnums , AllShopProperties> allShopItems = {
    {
        WorldEnums::CenterWorld,
        {
            {
                "shop1",
                {
                    {
                        "meat",
                        "resources/image/UI/shop/meat_icon.png",
                        50,
                        10,
                        50,
                        "give you alot of protient | price 50 | heal +50"
                    },
                    {
                        "cake",
                        "resources/image/UI/shop/cake_icon.png",
                        20,
                        10,
                        30,
                        "price 30 | heal +20"
                    },
                    {
                        "cookies",
                        "resources/image/UI/shop/cookies_icon.png",
                        20,
                        10,
                        30,
                        "price 30 | heal +20"
                    },
                    {
                        "meat4",
                        "resources/image/UI/shop/meat_icon.png",
                        50,
                        10,
                        30,
                        "price 30 | heal +20"
                    },
                    {
                        "meat5",
                        "resources/image/UI/shop/meat_icon.png",
                        20,
                        10,
                        30,
                        "price 30 | heal +20"
                    },
                }
            },
        }
    }
};