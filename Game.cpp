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

	if (TTF_Init() == -1)
	{
		std::cout << "Error in SDL_TTF" << std::endl;
	}

	assets->AddTexture("player", "assets/player.png");
	assets->AddTexture("projectile", "assets/projectile.png");
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
	default:
		break;
	}
	
}

void Game::update()
{
	manager.refresh();
	manager.update();
	Vector2D playerPos(0.0f, 0.0f);
	

	if (!gameOver)
	{
		//player position before update for collision detection
		playerPos = player.getComponent<TransformComponent>().position;

		for (auto& e : enemies)
		{
			e->getComponent<SpriteComponent>().setFrameCount(frameCount);
			e->getComponent<SpriteComponent>().setSpeed(enemySpeed);
		}
	}


	if(!gameOver)
	{
		//prevents firing more than one porjectile at a time
		for (auto& p : p_projectiles)
		{
			if (p->getComponent<ColliderComponent>().tag == "Projectile_P")
			{
				fire = false;
			}
		}


		//fire projectile from player position
		if (fire)
		{
			Vector2D location(player.getComponent<TransformComponent>().position.x + 15.0f, player.getComponent<TransformComponent>().position.y);
			assets->CreatePlayerProjectile(location, Vector2D(0.0f, -3.0f), "Projectile_P");
			fire = false;
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
						break;
					}
				}

				chance--;
			}
		}


		
		
		
	}

	if (!gameOver)
	{
		std::stringstream ss;
		//collider between player and enemies projectile
		for (auto& e : e_projectiles)
		{
			for (auto& p : players)
			{
				if (Collision::AABB(p->getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider))
				{	
					e->destroy();
					lives--;
					ss << "Lives: " << lives;
					label_L.getComponent<UILabelComponent>().SetLabelText(ss.str(), "verdana");
					
					if (lives <= 0)
					{
						gameOver = true;
					}
				}
			}
		}

		//collsion dectection between player projectile and enemies
		for (auto& p : p_projectiles)
		{
			for (auto& e : enemies)
			{
				if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
				{
					std::cout<< e->getComponent<ColliderComponent>().collider.y << std::endl;
					score += 10;
					ss << "Score: " << score;
					label_S.getComponent<UILabelComponent>().SetLabelText(ss.str(), "verdana");
					p->destroy();
					e->destroy();
					break;
				}
			}
	}
		
	}

	frameCount++;

	if (!gameOver)
	{
		//all enemies defeated, starting next round
		if (enemies.size() == 0)
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
			Layout::ResetLayout(round);
			std::cout << round << std::endl;
		}
		else
		{
			//have enemies reached bottom of screen?
			for (auto& e : enemies)
			{
				if (e->getComponent<TransformComponent>().position.y >= 568)
				{
					gameOver = true;
				}
			}
		}

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
		
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	if (!gameOver)
	{
		for (auto& pp : p_projectiles)
		{
			pp->draw();
		}
		for (auto& ep : e_projectiles)
		{
			ep->draw();
		}
		for (auto& p : players)
		{
			p->draw();
		}
		for (auto& e : enemies)
		{
			e->draw();
		}
		for (auto& t : tiles)
		{
			t->draw();
		}
	}

	label_S.draw();
	label_L.draw();

	if (gameOver)
	{
		label_E.draw();
		label_Q.draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::checkAlive()
{
	if (gameOver && !gameEndCheck)
	{
		for (auto& pp : p_projectiles)
		{
			pp->destroy();
		}
		for (auto& ep : e_projectiles)
		{
			ep->destroy();
		}
		for (auto& p : players)
		{
			p->destroy();
		}
		for (auto& e : enemies)
		{
			e->destroy();
		}

		SDL_Color white = { 255, 255, 255 };
		label_E.addComponent<UILabelComponent>(190, 256, ("Game Over"), "verdana", white);
		label_Q.addComponent<UILabelComponent>(190, 272, ("Esc to Quit"), "verdana", white);

		gameEndCheck = true;
	}
}

void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Game Cleaned!..." << std::endl;
}