#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "buttonEnums.h"
#include "../helpers.h"

class Button {
    const char* text;
    Vector2 location;
    Vector2 dimension;
    Color textColor = GetColor(THEMECOLOR);
    MenuActionEnums action;
    public:
        Button(const char* text, Vector2 location, MenuActionEnums action);
        void draw();
        Rectangle getButtonRec();
        MenuActionEnums getAction();
};
class UI {
        bool isGameStart;
        Vector2 location;
        Texture2D BackgroundTexture;
        Button startButton;
        Button saveButton;
        Button loadButton;
        std::vector<Button*> Buttons{};
    public:
        UI();
        void draw();
        void drawComponents();
        void startGame();
        std::vector<Button*> getMenuButton ();
};
class GameplayUI {
    public:
        GameplayUI();
};