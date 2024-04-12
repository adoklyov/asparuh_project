#include "Arrow.h"

Arrow::Arrow()
{
    arrowState = VISIBLE;
    visible = nullptr;
    invisible = nullptr;
    arrow = {0, 0, 100, 50};
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
    arrow.x = rectArrow.x;
    arrow.y = rectArrow.y;
    arrow.w = rectArrow.w;
    arrow.h = rectArrow.h;
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