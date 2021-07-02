#include <iostream>

#include "Game.hpp"

Game *game = nullptr;

const float TICK_LENGTH = 1.0/60.0;

int main() {
    game = new Game();
    game->init("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

    uint ts, prev;
    float dt, lag;
    while (game->running())
    {
        ts = SDL_GetTicks();
        if (!prev) prev = ts;
        dt = (ts - prev)/1000.0f;
        prev = ts;
        lag += dt;

        game->handleEvents();
        while (lag >= TICK_LENGTH) {
            game->update();
            lag -= TICK_LENGTH;
        }

        game->render(lag);
    }

    return 0;
}
