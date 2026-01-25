#include <iostream>
#include <raylib.h>

class UIComponent {
        std::function<void()> action;
        Rectangle dimension;
        std::string title;
    public:
        UIComponent(Rectangle dimension, std::string title, std::function<void()> action);
        void callAction();
        void draw();
};