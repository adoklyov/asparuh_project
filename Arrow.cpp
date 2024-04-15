#include "Arrow.h"


Arrow::Arrow() 
{
    arrowState = VISIBLE;
    visible = nullptr;
    invisible = nullptr;
    currentPosition = 0;
    arrow = {300, 300, 100, 50};
    fixedPositions [0] = {0, 0, 100, 50};
    fixedPositions [1] = {200, 150, 100, 50};
    fixedPositions [2] = {400, 150, 100, 50};
    setPosition(fixedPositions[currentPosition]);
    
}

void Arrow::changePosition()
{
    currentPosition = (currentPosition + 1) % 3;
    setPosition(fixedPositions[currentPosition]);
}

Arrow::~Arrow()
{
    SDL_DestroyTexture(visible);
    SDL_DestroyTexture(invisible);
}

void Arrow::render(SDL_Renderer *renderer)
{
    SDL_Texture *currentTexture = nullptr;
    currentTexture = visible;
    if (currentTexture != nullptr)
    {
        SDL_RenderCopy(renderer, currentTexture, NULL, &arrow);
    }
}

void Arrow::setVisibleTexture(SDL_Texture *texture)
{
    visible = texture;
}

void Arrow::setInvisibleTexture(SDL_Texture *texture)
{
    invisible = texture;
}

void Arrow::setPosition(SDL_Rect& rectArrow)
{
    arrow = rectArrow;
}

SDL_Rect Arrow::getArrowRect() const
{
    return this->arrow;
}

void Arrow::setActive(bool active)
{
    if (active)
    {
        arrowState = VISIBLE;
    }
    else
    {
        arrowState = INVISIBLE;
    }
}

bool Arrow::isActive()
{
    return arrowState != INVISIBLE;
}

double Arrow::getSpeedY() const
{
    return speedY;
}

void Arrow::setSpeedY(double sy) 
{
    speedY = sy;
}