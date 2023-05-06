#pragma once
#include <map>
#include <string>
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"
#include "SDL2/SDL_ttf.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	void CreateWall(Vector2D pos, int width, int height, std::string id);
	void CreateEnemy(Vector2D pos, int width, int height, std::string id);
	void CreatePlayerProjectile(Vector2D pos, Vector2D velocity, std::string tag);
	void CreateEnemyProjectile(Vector2D pos, Vector2D velocity, std::string tag);

	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};