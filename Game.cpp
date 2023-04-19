#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"



Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event; 

std::vector<ColliderComponent*> Game::colliders;

int Game::mPosX = 0;
int Game::mPosY = 0;
int Game::mVelX = 0;
int Game::mVelY = 0;
int Game::width = 52;
int Game::height = 45;

auto& player(manager.addEntity());




Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }



    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout<< "Subsystems Initialised!..." << std::endl;

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        
        if (window)
        {
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0); 
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout<< "renderer created!"<< std::endl;
        }

        isRunning = true;
    } 

    map = new Map();


    //ecs implemetaion

    player.addComponent<TransformComponent>(40.0f, 580.0f);
    player.addComponent<SpriteComponent>("assets/Spaceship3.png");
    player.addComponent<KeyboardController>();
    

}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    
    
    switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

}

void Game::render()
{
    SDL_RenderClear(renderer);
    map->DrawMap();
 
    manager.draw();
    SDL_RenderPresent(renderer);
    
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<< "Game Cleaned"<< std::endl;
}