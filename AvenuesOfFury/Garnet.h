#pragma once
#include "EnemyCharacter.h"

class Garnet : public EnemyCharacter {
public:
	Garnet();
	ActionStruct handleAI(float elapsedTime);
};