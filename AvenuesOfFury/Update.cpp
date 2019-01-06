#include "pch.h"
#include "Engine.h"
#include <iostream>

using namespace std;

void Engine::update(float timeElapsed) {
	auto it = remove_if(enemies.begin(), enemies.end(), [](Character* e) { return !e->isActive; });
	enemies.erase(it, enemies.end());
	for_each(players.begin(), players.end(), [&](Character* p) { p->update(timeElapsed, players, enemies); });
	for_each(enemies.begin(), enemies.end(), [&](Character* e) { e->update(timeElapsed, players, enemies); });
}