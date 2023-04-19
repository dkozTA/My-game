#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include <vector>

class ColliderComponent;

class Game
{
public:
    Game(); 
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    bool running() { return isRunning; }
    void render(); 
    void clean();
    

    
    static SDL_Renderer* renderer;
    static SDL_Event event; 
    static std::vector<ColliderComponent*> colliders;
    
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 640;
    

private:
    int count = 0;
    bool isRunning = false;
    SDL_Window *window = NULL;

    //The X and Y offsets of the dot
    static int mPosX, mPosY;

    //The velocity of the dot
    static int mVelX, mVelY;

    static int height;
    static int width;
    
};

