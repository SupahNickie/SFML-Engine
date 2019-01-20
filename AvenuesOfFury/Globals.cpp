#include "pch.h"
#include <assert.h>
#include "Globals.h"

using namespace std;

Globals* Globals::gInstance = nullptr;

Globals::Globals() {
	assert(gInstance == nullptr);
	idSerial = 0;
	gInstance = this;
}

Globals::ActionType Globals::actionStringToEnum(string const& action) {
	if (action == "move") return Globals::ActionType::MOVE;
	if (action == "attack") return Globals::ActionType::ATTACK;
	if (action == "idle") return Globals::ActionType::IDLE;
	if (action == "injure") return Globals::ActionType::INJURE;
	if (action == "jump_start") return Globals::ActionType::JUMP_START;
	if (action == "jump_air") return Globals::ActionType::JUMP_AIR;
	if (action == "jump_land") return Globals::ActionType::JUMP_LAND;
	if (action == "jump_attack") return Globals::ActionType::JUMP_ATTACK;
	if (action == "run_attack") return Globals::ActionType::RUN_ATTACK;
	if (action == "run_attack_land") return Globals::ActionType::RUN_ATTACK_LAND;
	if (action == "fall") return Globals::ActionType::FALL;
	if (action == "rise") return Globals::ActionType::RISE;
	if (action == "grab") return Globals::ActionType::GRAB;
	if (action == "grab_attack_head") return Globals::ActionType::GRAB_ATTACK_HEAD;
	if (action == "grab_attack_body") return Globals::ActionType::GRAB_ATTACK_BODY;
	if (action == "throw") return Globals::ActionType::THROW;
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

unsigned int Globals::getAndIncrementIDSerial() {
	return ++gInstance->idSerial;
}

bool Globals::isJumpingState(Globals::ActionType action) {
	return (
		action == Globals::ActionType::JUMP_START ||
		action == Globals::ActionType::JUMP_AIR ||
		action == Globals::ActionType::JUMP_ATTACK ||
		action == Globals::ActionType::JUMP_LAND
	);
}