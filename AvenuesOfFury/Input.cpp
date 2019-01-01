#include "pch.h"
#include "Engine.h"

using namespace std;

void Engine::input() {
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::KeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}
		}
	}
	for_each(players.begin(), players.end(), [](PlayerCharacter* p) { p->handleInput(); });
}