#pragma once
#include <iostream>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "AssetManager.hpp"

class AssetManager;
class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void checkAlive();
	void clean();
	void mainMenu();
	void pause();


	static bool isRunning;
	bool running() { return isRunning; }

	static bool fire;

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static AssetManager* assets;

	enum groupLabels : std::size_t
	{
		groupPlayer,
		groupEnemies,
		groupMap,
		groupColliders,
		groupEnemyProjectiles,
		groupPlayerProjectiles
	};

	static const int WINDOW_WIDTH = 460;
	static const int WINDOW_HEIGHT = 640;

	static Mix_Chunk* soundEffects[7];
	static Mix_Music* BackgroundMusic;

private:
	bool PauseRunning = false;
	bool menuRunning = true;
	bool gameOver = false;
	bool gameEndCheck = false;
	int frameCount = 0;
	int enemySpeed = 60;
	int score = 0;
	int lives = 3;
	int randNum = 0;
	int round = 0;
	bool reverseDirection = false;
	bool reverseCheck = false;
	SDL_Window *window;
};

