#include "pch.h"
#include "Engine.h"

void Engine::draw() {
	window.clear(Color::White);
	window.setView(mainView);
	for (int i = 0; i < numEnemies; ++i) {
		if (enemies[i]->isActive) {
			window.draw(*enemies[i]);
		}
	}
	for (int i = 0; i < numPlayers; ++i) {
		window.draw(*players[i]);
	}
	window.display();
}