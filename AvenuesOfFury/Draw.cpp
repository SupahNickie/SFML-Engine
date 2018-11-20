#include "pch.h"
#include "Engine.h"

void Engine::draw() {
	window.clear(Color::White);
	window.setView(mainView);
	window.draw(player1);
	window.display();
}