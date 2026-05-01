#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <iostream>
#include <raylib.h>

struct ButtonParameter {
    std::string name;
    std::function<void()> action;
};
struct ButtonParameter2 {
    std::string name;
    std::function<void()> action;
    Rectangle srcDimension;
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
class CategoryTabButton: public BaseButton {
    Texture2D& texture;
    bool isTrigger;
    Rectangle srcDimension;
    public:
        CategoryTabButton (Texture2D& texture, Rectangle srcDimension, ButtonParameter buttonDetail, Rectangle dimension, int index);
        virtual void draw () override;
        void setIsTrigger(bool isTrigger);
        bool getIsTrigger();
};
#endif