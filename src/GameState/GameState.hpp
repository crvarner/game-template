#pragma once

#include "SDL.h"

class GameState {
public:
    virtual void handleInput() {};
    virtual void update(float dt) {};
    virtual void render(float lag) = 0;
};

class IntroState: public GameState {
public:
    IntroState();
    void update(float dt);
    void render(float lag);

private:
    float elapsedTime = 0;
    SDL_Texture* background;
};

class MainMenuState: public GameState {
public:
    MainMenuState();
    void render(const float dt);
private:
    SDL_Texture* background;
    SDL_Texture* button;
    SDL_Texture* title;
};

class PlayState: public GameState {
public:
    void render(float lag);
};

