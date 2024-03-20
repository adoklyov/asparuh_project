#ifndef BUTTON_H
#define BUTTON_H

enum Buttons
{
    PRESSED,
    NOTPRESSED,
    INACTIVE
};

class Button
{
public:
    Button();

    bool isPressed() const;
    void setPressed(bool pressed);

private:
    bool pressed;
};



#endif