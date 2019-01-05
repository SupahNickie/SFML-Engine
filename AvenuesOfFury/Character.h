#pragma once
#include "Globals.h"
#include "Graphic.h"

using namespace sf;
using namespace std;

class Character : public Graphic {
protected:
	int MS_PER_FRAME = 50;
	int STUN_LENGTH = 100;
	int const MOVE_1 = 0;
	int const MOVE_2 = 1;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	int const IDLE_1 = 0;
	int const INJURE_1 = 0;

	Globals::ActionType spriteState;
	bool facingLeft;
	bool facingRight;
	float baseSpeed;
	float speed;
	bool jumping = false;
	bool running = false;

	int health = 0;
	vector<int> attackPower;
	bool attackDisabled = false;
	bool hitRegistered = false;

	bool currentActionDone = false;
	int currentFrame = 0;
	string currentAction;
	int currentActionType = 0;
	int timeSinceLastFrame = 0;
	int timeSinceLastAction = 0;
	bool spriteCycleDown = false;

	void resetFrameState();
	void updateFrameState(float elapsedTime);
	void render();
	virtual void setDirectionHeaded() = 0;
public:
	enum class DirectionHeaded { U, UR, R, DR, D, DL, L, UL, NONE };
	DirectionHeaded directionHeaded = DirectionHeaded::NONE;

	void flipHorizontally();
	DirectionHeaded stringToDirection(string const& direction);
	bool hits(Character* otherChar);
	void registerHit(int hp);
};