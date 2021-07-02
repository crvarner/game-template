#include <iostream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"

SDL_Renderer* Game::renderer = nullptr;
SDL_Texture* playerTex;
SDL_Rect srcRect, dstRect;
Map* map;

void Game::init(const char* title, int x, int y, int w, int h, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystem Initializing!..." << std::endl;

        window = SDL_CreateWindow(title, x, y, w, h, flags);
        if (window) {
            std::cout << "Window created!" << std::endl;
        }

        Game::renderer = SDL_CreateRenderer(window, -1, 0);
        if (Game::renderer) {
            SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            std::cout << "Renderer created!" << std::endl;
        }

        isRunning = true;
    }

    srcRect.x = srcRect.y = 0;
    srcRect.w = srcRect.h = 64;
    playerTex = TextureManager::LoadTexture("assets/player.png");
    map = new Map();
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

        default:
            break;
    }
}

void Game::update()
{
    count++;
    dstRect.x = dstRect.y = count;
    dstRect.w = dstRect.h = 64;
}

void Game::render(float lag)
{
    SDL_RenderClear(Game::renderer);
    map->DrawMap();
    TextureManager::Draw(playerTex, srcRect, dstRect);
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