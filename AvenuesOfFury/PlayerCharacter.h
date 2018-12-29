#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <vector>
#include "Character.h"
#include "EnemyCharacter.h"

class EnemyCharacter;
class PlayerCharacter : public Character {
protected:
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool primaryAttackPressed;
	bool secondaryAttackPressed;
	bool inputsDisabled = false;
	int timeSinceLastAction = 0;
private:
	void handleAttack(float elapsedTime);
public:
	virtual void handleInput() = 0;
	void update(float elapsedTime, vector<EnemyCharacter*> enemies);
	void disableInputs();
};

#endif