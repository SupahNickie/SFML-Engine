#include "pch.h"
#include "Engine.h"

void Engine::update(float timeElapsed) {
	for (int i = 0; i < numCharacters; ++i) {
		characters[i]->update(timeElapsed);
	}
}