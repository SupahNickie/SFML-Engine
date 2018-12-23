#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

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
	bool attackDisabled = false;
private:
	void handleAttack();
public:
	virtual void handleInput() = 0;
	void update(float elapsedTime, EnemyCharacter** enemies, int numEnemies);
};

#endif