#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

using namespace std;

class Globals {
public:
	enum class ActionType { MOVE, ATTACK, IDLE, NONE };

	static ActionType actionStringToEnum(string const& action);
};

#endif