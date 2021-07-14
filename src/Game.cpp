#include <iostream>

#include "SDL_image.h"
#include "lua.hpp"

#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"

// static int world (lua_State *L) {
//     const char *in = lua_tostring(L, 1);
//     char out[32];
//     sprintf(out, "c++(%s)\n", in);
//     lua_pushstring(L, out);
//     return 1;
// }
// lua_register(L, "world", world);
// lua_getglobal(L, "hello");
// lua_pushstring(L, "ARG");
// lua_pcall(L, 1, 1, 0);
// const char *out = lua_tostring(L, 1);
// std::cout << "out: " << out << std::endl;

// count = 0;
// srcRect.x = srcRect.y = 0;
// srcRect.w = srcRect.h = 64;
// playerTex = TextureManager::LoadTexture("assets/player.png");
// map = new Map();

SDL_Renderer* Game::renderer = nullptr;
GameState *Game::gameState = nullptr;
GameState *Game::nextState = nullptr;

void Game::init(const char* title, int x, int y, int w, int h, bool fullscreen)
{
    int flags = SDL_WINDOW_HIDDEN;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!window) {
        std::cout << "Window not created!" << std::endl;
        return;
    }

    Game::renderer = SDL_CreateRenderer(window, -1, 0);
    if (!Game::renderer) {
        std::cout << "Renderer not created!" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_ShowWindow(window);

    int IMG_TYPES = IMG_INIT_PNG;
    if (IMG_Init(IMG_TYPES) != IMG_TYPES) {
        std::cout << "IMG_Init failed" << std::endl;
        return;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, "scripts/main.lua")) {
        std::cout << "failed to load lua script: " << lua_tostring(L, -1) << std::endl;
        return;
    }

    if (lua_pcall(L, 0, 0, 0)) {
        std::cout << "failed to call lua script: " << lua_tostring(L, -1) << std::endl;
        return;
    }

    gameState = new IntroState();
    isRunning = true;
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            break;

        case SDL_KEYUP:
            break;

        default:
            break;
    }
}

void Game::SetState(GameState *newState)
{
    if (!nextState) {
        nextState = newState;
    }
}

void Game::update(float dt)
{
    if (nextState) {
        gameState = nextState;
        nextState = nullptr;
    }
    gameState->update(dt);
}

void Game::render(float lag)
{
    SDL_RenderClear(Game::renderer);
    gameState->render(lag);
    SDL_RenderPresent(Game::renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Game::renderer);
    SDL_Quit();
}

bool Game::running()
{
    return isRunning;
}