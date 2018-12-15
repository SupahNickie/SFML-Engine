#pragma once
#include "Globals.h"
#include "Graphic.h"

using namespace sf;
using namespace std;

struct ActionStruct {
	Globals::ActionType action;
	int actionType;
};

class Character : public Graphic {
protected:
	FloatRect hitbox;

	int MS_PER_FRAME = 50;
	int const MOVE_1 = 0;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	int const IDLE_1 = 0;
	float speed;

	enum class SpriteState { MOVING, ATTACKING, IDLE };

	SpriteState spriteState;
	bool facingLeft;
	bool facingRight;
	bool jumping;

	int moveFrame = 0;
	int attackFrame = 0;
	int idleFrame = 0;
	int timeSinceMoveFrame = 0;
	int timeSinceAttackFrame = 0;
	int timeSinceIdleFrame = 0;
	bool moveSpriteCycleDown;
	bool attackSpriteCycleDown;
	bool idleSpriteCycleDown;
	int timeSinceHandlingLastAttackFrame = 0;

	void renderMove(float elapsedTime, int moveType);
	void renderAttack(float elapsedTime, int attackType);
	void renderIdle(float elapsedTime, int idleType);
	
	void resetMoveFrame(int moveType);
	void resetAttackFrame(int attackType);
	void resetIdleFrame(int idleType);
	
	virtual void handleMove(float elapsedTime, int moveType) = 0;
	virtual void handleAttack(float elapsedTime, int attackType) = 0;
	virtual void handleIdle(float elapsedTime, int idleType) = 0;
public:
	void flipHorizontally();
	virtual void update(float elapsedTime) = 0;
};