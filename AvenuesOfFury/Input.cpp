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
			if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
				enemies[0]->deleteSprite();
			}
		}
	}
	for_each(players.begin(), players.end(), [](PlayerCharacter* p) { p->handleInput(); });
}