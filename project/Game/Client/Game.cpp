#include "pch.h"
#include "Game.h"

#include "Engine.h"

void Game::Init(const WindowInfo& window)
{
	GEngine->Init(window);
}

void Game::Update()
{
	GEngine->Render();
}
