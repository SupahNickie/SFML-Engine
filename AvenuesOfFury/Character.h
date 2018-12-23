#pragma once
#include "Globals.h"
#include "Graphic.h"

using namespace sf;
using namespace std;

class Character : public Graphic {
protected:
	FloatRect hitbox;

	int MS_PER_FRAME = 50;
	int const MOVE_1 = 0;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	int const IDLE_1 = 0;
	int const INJURE_1 = 0;
	float speed;

	Globals::ActionType spriteState;
	bool facingLeft;
	bool facingRight;
	bool jumping;

	bool currentActionDone = false;
	int currentFrame = 0;
	string currentAction;
	int currentActionType = 0;
	int timeSinceLastFrame = 0;
	bool spriteCycleDown = false;

	void resetFrameState();
	void updateFrameState(float elapsedTime);
	void render();
public:
	void flipHorizontally();
	bool hits(Character* otherChar);
};