#pragma once
#include "Game.hpp"
#include "ECS/ECS.hpp"

class Layout
{
public:
	Layout();
	void Draw();
	static void ResetLayout(int round);
};