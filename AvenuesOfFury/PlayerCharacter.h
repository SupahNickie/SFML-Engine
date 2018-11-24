#pragma once
#include "Character.h"

class PlayerCharacter : public Character {
protected:
	float speed;
	
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
	void update(float elapsedTime);
};