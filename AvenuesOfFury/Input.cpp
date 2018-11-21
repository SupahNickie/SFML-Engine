#include "pch.h"
#include "Engine.h"

void Engine::input() {
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::KeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}
		}
	}
	player1.handleInput();
	player2.handleInput();
}