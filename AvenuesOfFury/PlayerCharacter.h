#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Character.h"
#include "EnemyCharacter.h"

class EnemyCharacter;
class PlayerCharacter : public Character {
protected:
	int STUN_LENGTH = 100;

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
	void update(float elapsedTime, EnemyCharacter** enemies, int numEnemies);
	void disableInputs();
};

#endif