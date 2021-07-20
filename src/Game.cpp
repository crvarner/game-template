#include <iostream>
#include <map>

#include "SDL_image.h"
#include "lua.hpp"

#include "Game.hpp"
#include "TextureManager.hpp"

#include "ECS.hpp"
#include "Components/Components.hpp"

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

EntityManager *Game::manager = new EntityManager();
SDL_Renderer* Game::renderer = nullptr;
GameState *Game::gameState = nullptr;
GameState *Game::nextState = nullptr;

static void dumpstack (lua_State *L) {
    printf("================= DUMP =================\n");
    int top=lua_gettop(L);
    for (int i=1; i <= top; i++) {
        printf("%d\t%s\t", i, luaL_typename(L,i));
        switch (lua_type(L, i)) {

        case LUA_TNUMBER:
            printf("%g\n",lua_tonumber(L,i));
            break;
        
        case LUA_TSTRING:
            printf("%s\n",lua_tostring(L,i));
            break;
        
        case LUA_TBOOLEAN:
            printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
            break;
        
        case LUA_TNIL:
            printf("%s\n", "nil");
            break;
        
        default:
            printf("%p\n",lua_topointer(L,i));
            break;
        }
    }
}

static int createEntity(lua_State *L) {
    Entity *entity = new Entity();
    lua_settop(L, 1);
    lua_pushnil(L);
    dumpstack(L);
    while (lua_next(L, 1))
    {
        std::string type = lua_tostring(L, 2);
        //getLuaConstructor(type)(L);


        switch (strToComponentType(type)) {
            case IDLE:
                break;

            case POSITION:
                break;

            case SPRITE:
                break;

            case UNKNOWN:
                break;
        }
        lua_pop(L, 1);
    }
    Game::manager->addEntity(entity);
    return 0;
}

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

    lua_register(L, "createEntity", createEntity);
    lua_getglobal(L, "init");
    lua_pcall(L, 0, 0, 0);

    gameState = new IntroState();

    // Entity *player = new Entity();
    // Game::manager->addEntity(player);
    // player->addComponent<Sprite>("assets/player.png");
    // player->addComponent<Position>(100, 100, 100, 100);
    // player->addComponent<Idle>();

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
    // gameState->render(lag);
    Game::manager->render(lag);
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