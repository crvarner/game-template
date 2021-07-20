#include "SDL.h"

#include "Components/Components.hpp"

class Position: public Component {
public:
    Position(float x, float y, int w, int h);
    SDL_Rect getPos();

private:
    SDL_Rect pos;
};

Position::Position(float x, float y, int w, int h) {
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
}

SDL_Rect Position::getPos() {
    return pos;
}