#pragma once

#include "Components.hpp"
#include "../Vector2D.hpp"

struct TransformComponent : public Component
{

public:
    Vector2D position;

    Vector2D velocity;

    int height = 45;
    int width = 52;
    int scale = 1;

    int speed = 10;

    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }


    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y  += velocity.y * speed;
        
    }
};