#include "pch.h"
#include "Engine.h"

using namespace std;

void Engine::draw() {
	window.clear(Color::White);
	window.setView(mainView);
	for_each(enemies.begin(), enemies.end(), [&](Character* e) { window.draw(*e); });
	for_each(players.begin(), players.end(), [&](Character* p) { window.draw(*p); });
	window.display();
}