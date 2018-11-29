#include "pch.h"
#include "Engine.h"

void Engine::update(float timeElapsed) {
	for (int i = 0; i < numPlayers; ++i) {
		players[i]->update(timeElapsed);
	}
	for (int i = 0; i < numSprites; ++i) {
		sprites[i]->update(timeElapsed);
	}
}