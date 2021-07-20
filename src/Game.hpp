#pragma once

#include "SDL.h"

#include "ECS.hpp"
#include "GameState/GameState.hpp"

class Game {
public:
    Game() {};
    ~Game() {};

    void init(const char* title, int x, int y, int width, int height, bool fullscreen);
    void handleEvents();
    void update(float dt);
    void render(float lag);
    void clean();
    bool running();
    static void SetState(GameState *newState);

    static EntityManager *manager;
    static SDL_Renderer *renderer;

private:
    int count;
    bool isRunning;
    SDL_Window *window;

    static GameState *gameState;
    static GameState *nextState;
};