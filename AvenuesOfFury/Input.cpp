#include "pch.h"
#include "Engine.h"

void Engine::input() {
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::KeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}
			if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
				enemies[0]->deleteSprite();
			}
		}
	}
	for (int i = 0; i < numPlayers; ++i) {
		players[i]->handleInput();
	}
}