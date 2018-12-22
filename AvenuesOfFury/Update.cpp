#include "pch.h"
#include "Engine.h"

void Engine::update(float timeElapsed) {
	for (int i = 0; i < numPlayers; ++i) {
		players[i]->update(timeElapsed, enemies, numEnemies);
	}
	for (int i = 0; i < numEnemies; ++i) {
		if (enemies[i]->isActive) {
			enemies[i]->update(timeElapsed, players);
		}
	}
}