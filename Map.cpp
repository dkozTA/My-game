#include "Map.hpp"
#include "TextureManager.hpp" 

int lvl1[20][25] = {
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }

};

Map::Map()
{
      background = TextureManager::LoadTexture("assets/black.png");
      smt1 = TextureManager::LoadTexture("assets/stars.png");
      smt2 = TextureManager::LoadTexture("assets/background.png");

      LoadMap(lvl1);

      src.x = src.y = 0;
      src.w = dest.w = 32;
      src.h = dest.h = 32;


      dest.x = dest.y = 0;
}

Map::~Map()
{
      SDL_DestroyTexture(smt1);
      SDL_DestroyTexture(smt2);
      SDL_DestroyTexture(background);
}

void Map::LoadMap(int arr[20][25])
{
      for (int row = 0; row < 20; row++){
            for (int column = 0; column < 25; column++){
                  map[row][column] = arr[row][column];
            }
      }
}

void Map::DrawMap()
{
      int type = 0;

     for (int row = 0; row < 20; row++){
            for (int column = 0; column < 25; column++){
                  type = map[row][column];

                  dest.x = column*32;
                  dest.y = row*32;

                  switch (type)
                  {
                  case 0:
                        TextureManager::Draw(background, src, dest);
                        break;
                  
                  case 1:
                        TextureManager::Draw(smt1, src, dest);
                        break;

                  case 2:
                        TextureManager::Draw(smt2, src, dest);

                  default:
                        break;
                  }
            }
      }    
}