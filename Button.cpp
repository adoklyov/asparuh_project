#include "Button.h"

Button::Button()
{
    buttonState = ACTIVE;
    active = nullptr;
    pressed = nullptr;
    inactive = nullptr;
    button = {0, 0, 100, 50};
}

Button::~Button()
{
    SDL_DestroyTexture(active);
    SDL_DestroyTexture(inactive);
    SDL_DestroyTexture(pressed);
}

void Button::render(SDL_Renderer *renderer)
{
    SDL_Texture *currentTexture = nullptr;

    if (pressed && SDL_GetTicks() - pressTimer <= 200)
    {
        currentTexture = pressed;
    }
    else
    {
        switch (buttonState)
        {
        case ACTIVE:
            currentTexture = active;
            break;
        case PRESSED:
            currentTexture = pressed;
            break;
        case INACTIVE:
            currentTexture = inactive;
            break;
        }
    }

    if (currentTexture != nullptr)
    {
        SDL_RenderCopy(renderer, currentTexture, NULL, &button);
    }

    if (textTexture != nullptr)
    {
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect textRect = {button.x + (button.w - textWidth) / 2, button.y + (button.h - textHeight) / 2, textWidth, textHeight};

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
}

void Button::setText(std::string text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer)
{
    if (textTexture != nullptr)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface != nullptr)
    {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
    }
}

void Button::setActiveTexture(SDL_Texture *texture)
{
    active = texture;
}

void Button::setInactiveTexture(SDL_Texture *texture)
{
    inactive = texture;
}

void Button::setPressedTexture(SDL_Texture *texture)
{
    pressed = texture;
}

void Button::setPosition(SDL_Rect& rect)
{
    button.x = rect.x;
    button.y = rect.y;
    button.w = rect.w;
    button.h = rect.h;
}

SDL_Rect Button::getButtonRect() const
{
    return this->button;
}

void Button::setActive(bool active)
{
    if (active)
    {
        buttonState = ACTIVE;
    }
    else
    {
        buttonState = INACTIVE;
    }
}

bool Button::isActive()
{
    return buttonState != INACTIVE;
}

bool Button::isClicked(int mouseX, int mouseY)
{
    SDL_Point mousePoint = {mouseX, mouseY};
    return SDL_PointInRect(&mousePoint, &button);
}

void Button::setPressed()
{
    pressedB = true;
    pressTimer = SDL_GetTicks();
    buttonState = PRESSED;
}

void Button::delayPressed()
{
    if (pressedB && SDL_GetTicks() - pressTimer > 200)
    {
        pressedB = false;
        if (isActive())
        {
            buttonState = ACTIVE;
        }
        else
        {
            buttonState = INACTIVE;
        }
    }
}