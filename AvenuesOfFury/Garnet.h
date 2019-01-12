#pragma once
#include <vector>
#include "EnemyCharacter.h"

class Garnet : public EnemyCharacter {
public:
	Garnet(vector<Character*> players);
protected:
	bool handleAttacking(float elapsedTime);
	void handleMoving(float elapsedTime);
};