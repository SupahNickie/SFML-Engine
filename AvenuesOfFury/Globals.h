#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Globals {
private:
	static Globals* gInstance;
	Vector2f resolution;
	float scalingFactor;
public:
	enum class ActionType {
		MOVE,
		ATTACK,
		IDLE,
		INJURE,
		JUMP_START,
		JUMP_AIR,
		JUMP_LAND,
		JUMP_ATTACK,
		RUN_ATTACK,
		RUN_ATTACK_LAND,
		FALL,
		RISE,
		NONE
	};
	unsigned int idSerial;

	Globals();
	static ActionType actionStringToEnum(string const& action);
	static void setResolution(float x, float y);
	static Vector2f getResolution();
	static float getScalingFactor();
	static unsigned int getAndIncrementIDSerial();
	static bool isJumpingState(ActionType action);
};

#endif