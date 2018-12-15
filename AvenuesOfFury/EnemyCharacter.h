#pragma once
#include "Character.h"

class EnemyCharacter : public Character {
protected:
	void handleMove(float elapsedTime, int moveType);
	void handleAttack(float elapsedTime, int attackType);
	void handleIdle(float elapsedTime, int idleType);
public:
	virtual void update(float elapsedTime) = 0;
};