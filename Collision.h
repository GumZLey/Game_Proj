#pragma once

#include "SDL.h"
// AABB = Axis align bounding box

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};