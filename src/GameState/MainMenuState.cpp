#include "TextureManager.hpp"
#include "GameState/GameState.hpp"

MainMenuState::MainMenuState() {
    background = TextureManager::LoadTexture("assets/water.png");
    title = TextureManager::LoadTexture("assets/title.png");
    button = TextureManager::LoadTexture("assets/button.png");
}

void MainMenuState::render(float lag) {
    SDL_Rect src, dst;
    src.x = src.y = 0;
    src.w = src.h = 32;

    dst.x = dst.y = 0;
    dst.w = 800;
    dst.h = 640;
    TextureManager::Draw(background, src, dst);

    dst.x = 320;
    dst.w = dst.h = 160;
    TextureManager::Draw(title, src, dst);

    dst.y = 320;
    TextureManager::Draw(button, src, dst);
}