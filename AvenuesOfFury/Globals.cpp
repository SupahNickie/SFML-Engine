#include "pch.h"
#include "Globals.h"

using namespace std;

Globals::ActionType Globals::actionStringToEnum(string const& action) {
	if (action == "move") return Globals::ActionType::MOVE;
	if (action == "attack") return Globals::ActionType::ATTACK;
	if (action == "idle") return Globals::ActionType::IDLE;
	if (action == "injure") return Globals::ActionType::INJURE;
	return Globals::ActionType::NONE;
}