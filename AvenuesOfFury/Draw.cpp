#include "pch.h"
#include "Engine.h"

void Engine::draw() {
	window.clear(Color::White);
	window.setView(mainView);
	window.draw(player1);
	window.draw(player2);
	window.draw(player3);
	window.draw(player4);
	window.display();
}