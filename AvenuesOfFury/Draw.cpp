#include "pch.h"
#include "Engine.h"

void Engine::draw() {
	window.clear(Color::White);
	window.setView(mainView);
	for (int i = 0; i < numCharacters; ++i) {
		window.draw(*characters[i]);
	}
	window.display();
}