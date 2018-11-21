#include "pch.h"
#include "Engine.h"

void Engine::update(float timeElapsed) {
	player1.update(timeElapsed);
	player2.update(timeElapsed);
}