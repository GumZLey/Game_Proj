#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

auto& player(manager.addEntity());
///

auto& enemy(manager.addEntity());

///

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isrunning = false;

// const char* mapfile = "Assets/DungeonTileset.png";



Game::Game()
{

}

Game::~Game()
{

}



void Game::init(const char* title, int xpos, int ypos, int width, int height, int fullscreen)
{
	int flags = 0;
	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

	try
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		{
			std::cout << "SDL initialization succeeded!";

			window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
			if (window) std::cout << "Window initialize" << std::endl;

			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer)
			{
				SDL_SetRenderDrawColor(renderer, 32, 20, 20, 255);
				std::cout << "Renderer initialize" << std::endl;
			}

			isrunning = true;
		}
		else throw SDL_GetError();
	}
	catch (const char* s)
	{
		isrunning = false;
		std::cout << "Error: " << s << std::endl;
		SDL_Quit();
	}
	catch (...)
	{
		isrunning = false;
		std::cout << "It's error without any clues" << std::endl;
		SDL_Quit();
	}

	/*
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		isrunning = false;
		std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
		SDL_Quit();
	}
	else
	{
		std::cout << "SDL initialization succeeded!";

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) std::cout << "Window initialize" << std::endl;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 32, 20, 20, 255);
			std::cout << "Renderer initialize" << std::endl;
		}

		isrunning = true;
	}
	*/
	assets->AddTexture("terrain", "Assets/DungeonTileset.png");
	assets->AddTexture("player", "Assets/player_sprite.png");
	assets->AddTexture("projectile", "Assets/Bullet.png");

	assets->AddTexture("enemy", "Assets/barbarian_run_01.png");

	map = new Map("terrain", 4, 16);
	// ECS

	map->Loadmap("Assets/lasttilemap.map",25, 20);

	player.addComponent<TransformComponent>(100, 275, 20, 20, 6);
	player.addComponent<SpriteComponent>("player", false);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	enemy.addComponent<TransformComponent>(450, 350, 20, 20, 6);
	enemy.addComponent<SpriteComponent>("enemy", false);
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addComponent<EnemyComponent>(player.getComponent<TransformComponent>().velocity);
	enemy.addGroup(groupPlayers);



	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(0, 0), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(300, 600), Vector2D(2, -1), 200, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
//auto& enemy(manager.getGroup(Game::groupPlayers));

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isrunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}
	
	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			std::cout << "Hit player!" << std::endl;
			p->destroy();
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	enemy.getComponent<EnemyComponent>().player = player.getComponent<TransformComponent>().position;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > camera.w) camera.x = camera.w;
	if (camera.y > camera.h) camera.y = camera.h;
}



void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	/*
	for (auto& c : colliders)
	{
		c->draw();
	}
	
	*/

	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned" << std::endl;
}
