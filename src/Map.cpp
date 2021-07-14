#include "Map.hpp"
#include "TextureManager.hpp"

Map::Map()
{
    dirt = TextureManager::LoadTexture("assets/dirt.png");
    grass = TextureManager::LoadTexture("assets/grass.png");
    water = TextureManager::LoadTexture("assets/water.png");

    src.x = dst.x = 0;
    src.y = dst.y = 0;
    src.w = dst.w = 32;
    src.h = dst.h = 32;
}

void Map::LoadMap(int arr[20][25])
{
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 25; col++)
        {
            map[row][col] = arr[row][col];
        }
    }
}

void Map::DrawMap()
{
    int type = 0;
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 25; col++)
        {
            type = map[row][col];
            dst.x = col * 32;
            dst.y = row * 32;
            switch (type)
            {
                case 0:
                    TextureManager::Draw(water, src, dst);
                    break;

                case 1:
                    TextureManager::Draw(grass, src, dst);
                    break;

                case 2:
                    TextureManager::Draw(dirt, src, dst);
                    break;

                default:
                    break;
            }
        }
    }
}