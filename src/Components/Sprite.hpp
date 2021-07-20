#include "SDL.h"

#include "TextureManager.hpp"
#include "Components/Components.hpp"

class Sprite: public Component {
public:
    Sprite(const char *spritesheet);
    void render(float lag);
private:
    SDL_Texture *tex;
};

Sprite::Sprite(const char *spritesheet) {
    tex = TextureManager::LoadTexture(spritesheet);
}

void Sprite::render(float lag)
{
    SDL_Rect src;
    src.x = src.y = 0;
    src.w = src.h = 32;
    
    Position *pos = entity->getComponent<Position>();
    TextureManager::Draw(tex, src, pos->getPos());
}