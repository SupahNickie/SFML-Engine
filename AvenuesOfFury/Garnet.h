#pragma once
#include <vector>
#include "EnemyCharacter.h"

class Garnet : public EnemyCharacter {
protected:
	bool handleAttacking(float elapsedTime);
	void handleMoving(float elapsedTime);
public:
	Garnet(vector<Character*> players);
};