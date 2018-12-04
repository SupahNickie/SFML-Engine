#include "pch.h"
#include "Engine.h"

void Engine::draw() {
	window.clear(Color::White);
	window.setView(mainView);
	for (int i = 0; i < numSprites; ++i) {
		if (sprites[i]->isActive) {
			window.draw(*sprites[i]);
		}
	}
	for (int i = 0; i < numPlayers; ++i) {
		window.draw(*players[i]);
	}
	window.display();
}