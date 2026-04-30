#include "shop.h"
#include "../globalVar.h"
#include "../helpers.h"

InventoryCategory::InventoryCategory (ButtonParameter buttonDetail, Rectangle dimension, int index):
BaseButton(buttonDetail, dimension, index),
srcWidth(16),
srcHeight(16),
itemTexture(LoadTexture(
            ((std::string)("resources/image/UI/inventoryCategory/" + name + ".png")).c_str()
        )
    )
{

}
void InventoryCategory::draw () {
    DrawTexturePro(itemTexture, {0, 0, srcWidth, srcHeight},
        {dimension.x + 20, dimension.y + 10, srcHeight * 2, srcHeight * 2},
         {0, 0}, 0, WHITE);
}
ShopItem::ShopItem (const char* textureSrc, std::string name, int heal, int energyHeal, int price, std::string description): texture(LoadTexture(textureSrc)),
    name(name), heal(heal), energyHeal(energyHeal), price(price), description(description) {

}

ShopUIItem::ShopUIItem(Texture2D* texture, std::string name, int heal, int energyHeal, Rectangle dimension, int index, int price, std::string description):
    texture(texture), name(name), heal(heal), energyHeal(energyHeal),
    dimension(dimension), index(index),
    price(price), description(description) {
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
    categoryBackground(LoadTexture("resources/image/UI/inventory_category_bg.png")),
    iconTextureSet(LoadTexture("resources/image/Modern_UI_Style_1.png")),
    categoryWidth(80),
    categoryHeight(48),
    currentCategory("Inventory"),
    categoryState(CategoryState::Inventory),
    dimension {SCREEN_WIDTH/2 - 480*0.5, SCREEN_HEIGHT*0.5 - 320*0.5, 480, 320},
    categoryButtonYOffset(dimension.y - 32)
{
    int column{0};
    int temp_index{0};
    std::vector<ButtonParameter> categories = {
        {
            "Inventory",
            [this]() {
                this->currentCategory = "Inventory";
                this->categoryState = CategoryState::Inventory;
            }
        },
        {
            "Status",
            [this]() {
                this->currentCategory = "Status";
                this->categoryState = CategoryState::Status;
            }
        },
        {
            "Skill",
            [this]() {
                this->currentCategory = "Skill";
                this->categoryState = CategoryState::Skill;
            }
        },
        {
            "Weapon",
            [this]() {
                this->currentCategory = "Weapon";
                this->categoryState = CategoryState::Weapon;
            }
        },
        {
            "Setting",
            [this]() {
                this->currentCategory = "Setting";
                this->categoryState = CategoryState::Setting;
            }
        },
    };
    // "Inventory", "Status", "Skill", "Weapon", "Setting"
    Rectangle temp_dimension = {dimension.x + 40 + 70*column, dimension.y + 50, 50,50};

    for (int i = 0; i < categories.size(); i++) {
        std::string currentName = categories[i].name;
        Rectangle current_dimension = {dimension.x + categoryWidth * i, dimension.y - 22, static_cast<float>(categoryWidth), static_cast<float>(categoryHeight)};
        InventoryCategory newCategory(categories[i], current_dimension, i);
        allCategories.emplace(currentName, newCategory);
        std::cout<< "item cat " << i << std::endl << std::flush;
    };
    for (ShopItem &item: shopItems) {
        items.emplace_back(&item.texture, item.name, item.heal, item.energyHeal, temp_dimension, temp_index, item.price, item.description);
        column++;
        temp_dimension.x = temp_dimension.x + 40 + 70*column;
        temp_index++;
    }
}
void Inventory::tick () {
    handleCategoryClick();
    draw();
    switch (categoryState)
    {
    case CategoryState::Inventory:
        {
            drawInventory();
            updateCategoryButtonPos("Inventory");
        }
        break;
    case CategoryState::Status:
        {
            drawStatus();
            updateCategoryButtonPos("Status");
        }
        break;
    case CategoryState::Skill:
        {
            drawSkill();
            updateCategoryButtonPos("Skill");
        }
        break;
    case CategoryState::Weapon:
        {
            drawWeapon();
            updateCategoryButtonPos("Weapon");
        }
        break;
    case CategoryState::Setting:
        {
            drawSetting();
            updateCategoryButtonPos("Setting");
        }
        break;
    default:
        break;
    }
}
void Inventory::handleCategoryClick() {
    for (auto& category: allCategories) {
        bool isCollide = checkButtonClick(category.second.getDimension()).isCollide;
        if (isCollide) {
            category.second.doAction();
        }
    }
}
CategoryState& Inventory::getCategoryState() {
    return categoryState;
}
void Inventory::draw () {
    DrawText(currentCategory.c_str(), dimension.x + 20, dimension.y - 50, 20, WHITE);
    
    for (auto& category: allCategories) {
        DrawTexturePro(categoryBackground, {0,0, 80, 64}, 
            {category.second.getDimension().x, category.second.getDimension().y, category.second.getDimension().width, category.second.getDimension().height + 16}
            , {0,0}, 0, WHITE);
        category.second.draw();
    }
    DrawTexturePro(backgroundTexture, {0,0, 480, 320}, {dimension.x , dimension.y, 480, 320 }, {0,0}, 0, WHITE);
}
void Inventory::drawInventory() {
    for (ShopUIItem item: items) {
        Rectangle button_dimension = item.getDimension();
        if (checkMouseOnHover(button_dimension).isCollide) {
            drawButtonBackground(button_dimension, GetColor(HOVER_BACKGROUND_COLOR), 10, 3);
        }
        item.draw();
    }
}
void Inventory::drawSetting() {
    DrawTexturePro(iconTextureSet, {7 * 16,15 * 16, 16, 16}, {dimension.x + 40, dimension.y + 40, 16 * 2, 16 * 2 }, {0,0}, 0, WHITE);
    DrawTexturePro(iconTextureSet, {10 * 16,15 * 16, 16, 16}, {dimension.x + 80, dimension.y + 40, 16 * 2, 16 * 2 }, {0,0}, 0, WHITE);
    DrawText("Settings", dimension.x + 100, dimension.y + 100, 14, WHITE);
}
void Inventory::drawSkill() {
    DrawText("Skill", dimension.x + 100, dimension.y + 100, 14, WHITE);
}
void Inventory::drawStatus() {
    DrawText("Status", dimension.x + 100, dimension.y + 100, 14, WHITE);
}
void Inventory::drawWeapon() {
    DrawText("Weapon", dimension.x + 100, dimension.y + 100, 14, WHITE);
}
void Inventory::updateCategoryButtonPos(std::string targetCategory)
{
    for (auto &category : allCategories)
    {
        if (category.second.getName() == targetCategory)
        {
            if (category.second.getDimension().y > categoryButtonYOffset)
                category.second.updatePosition({category.second.getDimension().x, category.second.getDimension().y - 2});
        } else
        {
            if (category.second.getDimension().y < categoryButtonYOffset + 10)
                category.second.updatePosition({category.second.getDimension().x, category.second.getDimension().y + 2});
        }
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