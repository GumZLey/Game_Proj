#pragma once

#include "Game.h"

class GameObject
{
public:
	GameObject(const char* texturesheet, int x, int y);
	~GameObject();

	void Update();
	void Render();
private:
	int xpos, ypos;

	SDL_Texture* ObjectTexture;
	SDL_Rect srcRect, destRect;

};