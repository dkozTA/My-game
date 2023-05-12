#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"
#include "Layout.hpp"
#include <sstream>
#include <ctime>
#include <cstdlib>

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
AssetManager* Game::assets = new AssetManager(&manager);

Mix_Chunk* Game::soundEffects[7];
Mix_Music* Game::BackgroundMusic = NULL;

std::vector<ColliderComponent*> Game::colliders;

auto& players(manager.getGroup(Game::groupPlayer));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& tiles(manager.getGroup(Game::groupMap));
auto& p_projectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& e_projectiles(manager.getGroup(Game::groupEnemyProjectiles));

auto& player(manager.addEntity());
auto& label_S(manager.addEntity());
auto& label_L(manager.addEntity());
auto& label_E(manager.addEntity());
auto& label_Q(manager.addEntity());
auto& label_M(manager.addEntity());
auto& label_P(manager.addEntity());

bool Game::isRunning = false;
bool Game::fire = false;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Created!..." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer Created!..." << std::endl;
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        std::cout << "can't open audio" << std::endl;
    }

    soundEffects[0] = Mix_LoadWAV("assets/sound/gameStart.wav");
    soundEffects[1] = Mix_LoadWAV("assets/sound/playerShoot.wav");
    soundEffects[2] = Mix_LoadWAV("assets/sound/alienShoot.wav");
    soundEffects[3] = Mix_LoadWAV("assets/sound/alienExplosion.wav");
	soundEffects[4] = Mix_LoadWAV("assets/sound/gameOver.wav");
	soundEffects[5] = Mix_LoadWAV("assets/sound/roundReset.wav");
	soundEffects[6] = Mix_LoadWAV("assets/sound/playerHit.wav");


	BackgroundMusic = Mix_LoadMUS("assets/sound/music.mp3");

	if(Mix_PlayingMusic()==0)
    {
        Mix_PlayMusic(BackgroundMusic,-1);
    }
    else
    {
        if(Mix_PausedMusic()==1) Mix_ResumeMusic();
        else Mix_PauseMusic();
    }

	if (TTF_Init() == -1)
	{
		std::cout << "Error in SDL_TTF" << std::endl;
	}

	assets->AddTexture("player", "assets/images/player.png");
	assets->AddTexture("Projectile_E", "assets/images/projectile.png");
	assets->AddTexture("Projectile_P", "assets/images/projectile.png");
	assets->AddFont("verdana", "assets/verdana.ttf", 16);

	player.addComponent<TransformComponent>(240.0f, 600.0f, 16, 16, 2);
	player.addComponent<SpriteComponent>("player");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(Game::groupPlayer);
	

	SDL_Color white = { 255, 255, 255 };
	label_S.addComponent<UILabelComponent>(8, 8, ("Score: 0"), "verdana", white);
	label_L.addComponent<UILabelComponent>(408, 8, ("Lives: 3"), "verdana", white);

	Layout layout;
	layout.Draw();

	srand(time(0));
	randNum = rand() % 240;
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (gameOver){
				// press n to escape the game
				if (event.key.keysym.sym == SDLK_n){
					isRunning = false;
				}
				//press y to back to menu
				if (event.key.keysym.sym == SDLK_y){
					menuRunning = true;
					gameOver = false;
					score = 0;
					round = 0;
					lives = 3;
				}
			}
			else if (!gameOver){
				if (event.key.keysym.sym == SDLK_t){
					gameEndCheck = true;
				}
			}
			
			if (menuRunning){
				// Pressing play starts the game
				if (event.key.keysym.sym == SDLK_SPACE)
				{
						menuRunning = false;
				}
				else if (event.key.keysym.sym == SDLK_x){
						isRunning = false;
				}
			}
			if (!PauseRunning){
				//press Ecs to pause
				if (event.key.keysym.sym == SDLK_ESCAPE){
					PauseRunning = true;
				}
			}
			else if (PauseRunning){
				//press Ecs to not pause
				if (event.key.keysym.sym == SDLK_ESCAPE){
					PauseRunning = false;
				}
				else if (event.key.keysym.sym == SDLK_x){
					isRunning = false;
				}
			}
			break;
		default:
			break;
	}
	
}

void Game::update()
{
	if (!menuRunning){
		if (!PauseRunning){
			manager.refresh();
			manager.update();

			frameCount++;
			if (!gameOver){

				//enemies sprite
				for (auto& e : enemies)
				{
					e->getComponent<SpriteComponent>().setFrameCount(frameCount);
					e->getComponent<SpriteComponent>().setSpeed(enemySpeed);
				}
			

				

				//prevents firing more than one porjectile at a time
				for (auto& pp : p_projectiles)
				{
					if (pp->getComponent<ColliderComponent>().tag == "Projectile_P")
					{
						fire = false;
					}
				}

				if (fire){
					//fire projectile from player position
					Vector2D location(player.getComponent<TransformComponent>().position.x + 15.0f, player.getComponent<TransformComponent>().position.y);
					//std::cout << "Player Projectile Location: x = " << location.x << ", y = " << location.y << std::endl; //add this line
					assets->CreatePlayerProjectile(location, Vector2D(0.0f, -3.0f), "Projectile_P");
					fire = false;
					Mix_PlayChannel(-1, Game::soundEffects[1], 0);
				}


				std::stringstream ss;

				
				//enemy movement
				if (frameCount % enemySpeed == 0)
				{
					Vector2D vSide(8.0f, 0.0f);
					if (reverseDirection)
					{
						Vector2D vRev(-1.0f, 0.0f);
						vSide.Multiply(vRev);
					}

					//reverse direction if needed and speed up enemy movement + animation
					if (reverseCheck)
					{
						Vector2D vDown(0.0f, 48.0f);
						for (auto& e : enemies)
						{
							e->getComponent<TransformComponent>().position.Add(vDown);
						}

						if (enemySpeed > 20)
						{
							enemySpeed -= 10;
						}
						else if (enemySpeed > 10)
						{
							enemySpeed -= 5;
						}


						reverseDirection = !reverseDirection;
						reverseCheck = false;
					}
					else
					{
						for (auto& e : enemies)
						{
							e->getComponent<TransformComponent>().position.Add(vSide);

							if (e->getComponent<TransformComponent>().position.x <= 8 ||
								e->getComponent<TransformComponent>().position.x >= 438)
							{
								reverseCheck = true;
							}
						}
					}
				}

				//firing enemy projectiles at random intervals
				if (frameCount >= randNum)
				{
					randNum += rand() % (240 - (round * 10));
					int chance = 20;
					bool fired = false;

					while (!fired && chance > 0)
					{
						for (auto& e : enemies)
						{
							if (rand() % chance == 0)
							{
								Vector2D location(e->getComponent<TransformComponent>().position.x + 15.0f, e->getComponent<TransformComponent>().position.y);
								assets->CreateEnemyProjectile(location, Vector2D(0.0f, 2.0f), "Projectile_E");
								fired = true;
								Mix_PlayChannel(-1, Game::soundEffects[2], 0);
								break;
							}
						}

						chance--;
					}
				}

				//all enemies defeated, starting next round
				if (enemies.size() == 0 && !gameOver)
				{
					round++;
					score += 100;
					enemySpeed = 60 - (5 * round);
					if (enemySpeed < 10)
					{
						enemySpeed = 10;
					}
					reverseCheck = false;
					reverseDirection = false;
					Layout::ResetLayout(0);
					std::cout << round << std::endl;
					Mix_PlayChannel(-1, Game::soundEffects[5], 0);
				}
				else
				{
					//have enemies reached bottom of screen?
					for (auto& e : enemies)
					{
						if (e->getComponent<TransformComponent>().position.y >= 568)
						{
							gameOver = true;
							score = 0;
							round = 0;
							Mix_PlayChannel(-1, Game::soundEffects[4], 0);
						}
					}
				}

				//collider between player and enemies projectile
				for (auto& ep : e_projectiles)
				{
					if (Collision::AABB(player.getComponent<ColliderComponent>().collider, ep->getComponent<ColliderComponent>().collider))
					{	
						ep->destroy();
						lives--;
						if (lives < 0)
						{
							ss << "Lives: " << 3;
						}
						else
						{
							ss << "Lives: " << lives;
						}
						label_L.getComponent<UILabelComponent>().SetLabelText(ss.str(), "verdana");
						
						if (lives < 0)
						{
							gameOver = true;
							score = 0;
							round = 0;
							Mix_PlayChannel(-1, Game::soundEffects[4], 0);
						}
						Mix_PlayChannel(-1, Game::soundEffects[6], 0);
					}
				}
				

				//collsion dectection between player projectile and enemies
				for (auto& pp : p_projectiles)
				{
					for (auto& e : enemies)
					{
						if (Collision::AABB(pp->getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider))
						{
							pp->destroy();
							e->destroy();
							score += 10;
							std::cout<< "hit"<< std::endl;
							ss << "Score: " << score;
							label_S.getComponent<UILabelComponent>().SetLabelText(ss.str(), "verdana");
							Mix_PlayChannel(-1, Game::soundEffects[3], 0);
						}
					}
				}
			}
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

	if (menuRunning)
	{
		label_M.draw();
	}

	else if (PauseRunning)
	{
		label_P.draw();
	}
	
	else if (!gameOver)
	{
		
		for (auto& t : tiles)
		{
			t->draw();
		}
		for (auto& p : players)
		{
			p->draw();
		}
		for (auto& e : enemies)
		{
			e->draw();
		}
		for (auto& pp : p_projectiles)
		{
			pp->draw();
		}
		for (auto& ep : e_projectiles)
		{
			ep->draw();
		}
		label_S.draw();
		label_L.draw();
	}


	if (gameOver)
	{
		label_E.draw();
		label_Q.draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::mainMenu()
{
      
      // Render menu
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

	  SDL_Color white = { 255, 255, 255 };

      if (menuRunning)
	  {
		// Render text and graphics for menu
		label_M.addComponent<UILabelComponent>(1, 1, ("product of Quang The Anh-22026554"), "verdana", white);
		label_M.addComponent<UILabelComponent>(190, 256, ("SPACE INVADER"), "verdana", white);
		label_M.addComponent<UILabelComponent>(160, 276, ("PRESS SPACE TO START"), "verdana", white);
		label_M.addComponent<UILabelComponent>(180, 296, ("PRESS X TO QUIT"), "verdana", white);
	  }
      SDL_RenderPresent(renderer);
}

void Game::pause()
{
	// Render pause screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Color white = { 255, 255, 255 };

    if (!PauseRunning)
	{
		// Render text and graphics for pause
		label_P.addComponent<UILabelComponent>(190, 256, ("Game Paused!"), "verdana", white);
		label_P.addComponent<UILabelComponent>(160, 276, ("Press Ecs To Continue"), "verdana", white);
		label_P.addComponent<UILabelComponent>(185, 296, ("Press X To Quit"), "verdana", white);
	}
	  
    SDL_RenderPresent(renderer);
}

void Game::checkAlive()
{
	if (gameOver)
	{
		for (auto& e : enemies)
		{
			e->destroy();
		}
		for (auto& ep : e_projectiles)
		{
			ep->destroy();
		}
		for (auto& pp : p_projectiles)
		{
			pp->destroy();
		}

		SDL_Color white = { 255, 255, 255 };
		label_E.addComponent<UILabelComponent>(190, 256, ("Game Over"), "verdana", white);
		label_Q.addComponent<UILabelComponent>(110, 272, ("Press y to back to menu, n to quit."), "verdana", white);

	}

	else if (gameEndCheck){
		for (auto& e : enemies)
		{
			e->destroy();
		}
		for (auto& ep : e_projectiles)
		{
			ep->destroy();
		}
		for (auto& pp : p_projectiles)
		{
			pp->destroy();
		}

		gameEndCheck = false;
	}
}

void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Game Cleaned!..." << std::endl;
}