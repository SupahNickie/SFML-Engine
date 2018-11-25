#pragma once
#include "Character.h"

class PlayerCharacter : public Character {
protected:
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool primaryAttackPressed;
	bool secondaryAttackPressed;
	bool attackDisabled = false;
private:
	void handleMove(float elapsedTime, int moveType);
	void handleAttack(float elapsedTime, int attackType);
	void handleIdle(float elapsedTime, int idleType);
public:
	virtual void handleInput() = 0;
	void update(float elapsedTime);
};