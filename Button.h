#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

enum Buttons
{
    ACTIVE,
    PRESSED,
    INACTIVE
};

class Button

{

public:
    Button();
    ~Button();

    void render(SDL_Renderer *renderer);
    void setText(std::string text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer);
    void setPosition(SDL_Rect& rect);

    void setActiveTexture(SDL_Texture *active);
    void setPressedTexture(SDL_Texture *pressed);
    void setInactiveTexture(SDL_Texture *inactive);

    SDL_Rect getButtonRect() const;
    void setActive(bool active);
    bool isActive();
    bool isClicked(int mouseX, int mouseY);

    void setPressed();
    void delayPressed();

private:
    SDL_Rect button;
    Buttons buttonState;

    SDL_Texture *active;
    SDL_Texture *pressed;
    SDL_Texture *inactive;

    SDL_Texture *textTexture;

    Uint32 pressTimer;
    bool pressedB;
};