#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int x, int y)
{
	xpos = x;
	ypos = y;

	ObjectTexture = TextureManager::LoadTexture(texturesheet);

}

void GameObject::Update()
{
	xpos++;
	ypos++;

	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
	destRect.x = xpos;
	destRect.y = ypos;

}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, ObjectTexture, &srcRect, &destRect);
}