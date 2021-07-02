#include "SDL_image.h"

#include "TextureManager.hpp"
#include "Game.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* filename)
{
    SDL_Surface* tmpSurface = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst)
{
    SDL_RenderCopy(Game::renderer, tex, &src, &dst);
}