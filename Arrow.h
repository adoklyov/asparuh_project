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

    double getSpeedY() const;
    void setSpeedY(double sy);

    void changePosition();

    void setActive(bool active);
    bool isActive();

    SDL_Rect getArrowRect() const;

 

private:
    SDL_Rect arrow;
    Arrows arrowState;

    SDL_Texture *visible;
    SDL_Texture *invisible;

    //Index for current position
    int currentPosition;

    //Positions
    SDL_Rect fixedPositions[3];

    //Animation speed
    double speedY;
};