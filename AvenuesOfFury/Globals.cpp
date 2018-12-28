#include "pch.h"
#include <assert.h>
#include "Globals.h"

using namespace std;

Globals* Globals::gInstance = nullptr;

Globals::Globals() {
	assert(gInstance == nullptr);
	gInstance = this;
}

Globals::ActionType Globals::actionStringToEnum(string const& action) {
	if (action == "move") return Globals::ActionType::MOVE;
	if (action == "attack") return Globals::ActionType::ATTACK;
	if (action == "idle") return Globals::ActionType::IDLE;
	if (action == "injure") return Globals::ActionType::INJURE;
	return Globals::ActionType::NONE;
}

void Globals::setResolution(float x, float y) {
	gInstance->resolution.x = x;
	gInstance->resolution.y = y;
	gInstance->scalingFactor = x / 1920;
}

Vector2f Globals::getResolution() {
	return gInstance->resolution;
}

float Globals::getScalingFactor() {
	return gInstance->scalingFactor;
}