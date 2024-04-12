#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

enum Arrows
{
    VISIBLE,
    INVISIBLE
};

class Arrow

{

public:
    Arrow();
    ~Arrow();

    void render(SDL_Renderer *renderer);
    void setPosition(SDL_Rect& rectArrow);

    void setVisibleTexture(SDL_Texture *visible);
    void setInvisibleTexture(SDL_Texture *invisible);

    void setActive(bool active);
    bool isActive();

    SDL_Rect getArrowRect() const;

private:
    SDL_Rect arrow;
    Arrows arrowState;

    SDL_Texture *visible;
    SDL_Texture *invisible;

};