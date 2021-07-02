#pragma once

#include "SDL.h"
#include "SDL_image.h"

class Game {
public:
    Game() {};
    ~Game() {};

    void init(const char* title, int x, int y, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render(float lag);
    void clean();
    bool running();
    static SDL_Renderer *renderer;

private:
    int count;
    bool isRunning;
    SDL_Window *window;
};