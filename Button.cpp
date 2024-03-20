#include "Button.h"

Button::Button()
{
}

bool Button::isPressed() const
{
    return this->pressed;
}

void Button::setPressed(bool pressed)
{
    this->pressed = pressed;
}