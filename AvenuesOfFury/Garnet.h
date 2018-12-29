#pragma once
#include <vector>
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"

class Garnet : public EnemyCharacter {
protected:
	int const MOVE_WALK = MOVE_1;
	int const ATTACK_SLAP = ATTACK_1;
	int const IDLE_HAIR = IDLE_1;

public:
	Garnet(vector<PlayerCharacter*> players);
	void handleAI(float elapsedTime, vector<PlayerCharacter*> players);
};