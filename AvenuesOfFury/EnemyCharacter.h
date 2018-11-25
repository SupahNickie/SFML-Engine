#pragma once
#include "Character.h"

class EnemyCharacter : public Character {
private:
	void handleMove(float elapsedTime, int moveType);
	void handleAttack(float elapsedTime, int attackType);
	void handleIdle(float elapsedTime, int idleType);
public:
	void handleInput();
	void update(float elapsedTime);
};