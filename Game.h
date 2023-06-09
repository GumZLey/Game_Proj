#ifndef Game_h
#define Game_h

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class AssetManager;
class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, int fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isrunning; };
	
	static SDL_Renderer* renderer;
	static SDL_Event event;

	static bool isrunning;
	static SDL_Rect camera;
	static AssetManager* assets;


	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles
	};
private:
	int cnt = 0;
	SDL_Window *window;
};


#endif // !Game_h
