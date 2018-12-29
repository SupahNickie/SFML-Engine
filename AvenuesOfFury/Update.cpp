#include "pch.h"
#include "Engine.h"
#include <iostream>

using namespace std;

void Engine::update(float timeElapsed) {
	for_each(players.begin(), players.end(), [&](PlayerCharacter* p) { p->update(timeElapsed, enemies); });
	auto it = remove_if(enemies.begin(), enemies.end(), [](EnemyCharacter* e) { return !e->isActive; });
	enemies.erase(it, enemies.end());
	for_each(enemies.begin(), enemies.end(), [&](EnemyCharacter* e) { e->update(timeElapsed, players); });
}