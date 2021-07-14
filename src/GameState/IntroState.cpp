#include <iostream>

#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameState/GameState.hpp"

IntroState::IntroState() {
    background = TextureManager::LoadTexture("assets/button.png");
}

void IntroState::update(float dt) {
    elapsedTime += dt;
    if (elapsedTime > 2) {
        Game::SetState(new MainMenuState());
    }
}

void IntroState::render(float lag) {
    SDL_Rect src, dst;
    src.x = src.y = dst.x = dst.y = 0;
    src.w = src.h = 32;
    dst.w = 800;
    dst.h = 640;

    TextureManager::Draw(background, src, dst);
}