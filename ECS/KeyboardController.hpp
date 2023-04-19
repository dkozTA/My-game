#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
public:
    TransformComponent *transform;
    

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        if(Game::event.type == SDL_KEYDOWN){
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_UP:
                transform->velocity.y = -1;
                break;
            
            case SDLK_LEFT:
                transform->velocity.x = -1;
                break;

            case SDLK_RIGHT:
                transform->velocity.x = 1;
                break;

            case SDLK_DOWN:
                transform->velocity.y = 1;
                break;
            
            default:
                break;
            }
        }
        if (Game::event.type == SDL_KEYUP){
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_UP:
                transform->velocity.y = 0;
                break;
            
            case SDLK_LEFT:
                transform->velocity.x = 0;
                break;

            case SDLK_RIGHT:
                transform->velocity.x = 0;
                break;

            case SDLK_DOWN:
                transform->velocity.y = 0;
                break;
            
            default:
                break;
            }
        }
        
        //change 18/4/2023
        float xpos = transform->position.x;
        float ypos = transform->position.y;

        if(xpos < 0)
            transform->position.x = 0;

        if(ypos < 0)
            transform->position.y = 0;

        if(xpos > Game::WINDOW_WIDTH - transform->width)
            transform->position.x = Game::WINDOW_WIDTH - transform->width;

        if(ypos > Game::WINDOW_HEIGHT - transform->height)
            transform->position.y = Game::WINDOW_HEIGHT - transform->height;

    }

    
};