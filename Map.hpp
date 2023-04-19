#pragma once

#include "Game.hpp"

class Map
{
public:
    Map();
    ~Map();

    void LoadMap(int arr[20][25]);
    void DrawMap();

private:
    SDL_Rect src, dest;
    
    SDL_Texture* smt2;
    SDL_Texture* smt1;
    SDL_Texture* background;

    int map[20][25];
};