#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <iostream>
#include <raylib.h>

struct ButtonParameter {
    std::string name;
    std::function<void()> action;
};
class BaseButton {
    protected:
    std::string name;
    Rectangle dimension;
    int index;
    std::function<void()> action;
    public:
        BaseButton(ButtonParameter buttonDetail, Rectangle dimension, int index);
        void doAction();
        virtual void draw();
        Rectangle& getDimension();
        int getIndex();
        void updatePosition(Vector2 pos);
        std::string& getName();
};

#endif