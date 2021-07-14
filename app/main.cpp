#if defined(_WIN32) && defined(_DEBUG)
#include <iostream>
#include "windows.h"
void init_logging() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}
#else
void init_logging() {}
#endif

#include "Game.hpp"

Game *game = nullptr;

const float TICK_LENGTH = 1.0/60.0;

int main(int argc, char** argv) {
    init_logging();
    game = new Game();
    game->init("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

    unsigned int ts, prev = 0;
    float dt, lag = 0;
    while (game->running())
    {
        ts = SDL_GetTicks();
        if (!prev) prev = ts;
        dt = (ts - prev)/1000.0f;
        prev = ts;
        lag += dt;

        game->handleEvents();
        while (lag >= TICK_LENGTH) {
            game->update(TICK_LENGTH);
            lag -= TICK_LENGTH;
        }

        game->render(lag);
    }

    return 0;
}
