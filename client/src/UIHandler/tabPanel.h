#include <string.h>
#include <iostream>
#include <raylib.h>
#include "button.h"

class TabPanel {
    std::unordered_map<std::string, Texture2D> textureSets;
    std::unordered_map<std::string, CategoryTabButton> tabButtons;
    public:
        TabPanel(ButtonParameter, std::vector<std::string> texturePaths);
};