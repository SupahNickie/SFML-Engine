#pragma once
#include <vector>
#include "EnemyCharacter.h"

class Garnet : public EnemyCharacter {
protected:
	int const MOVE_WALK = MOVE_1;
	int const ATTACK_SLAP = ATTACK_1;
	int const IDLE_HAIR = IDLE_1;

	bool handleAttacking(float elapsedTime);
	void handleMoving(float elapsedTime);
public:
	Garnet(vector<Character*> players);
};