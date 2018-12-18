#pragma once
#include "Character.h"

class EnemyCharacter : public Character {
protected:
	void handleMove(float elapsedTime, int moveType);
	void handleAttack(float elapsedTime, int attackType);
	void handleIdle(float elapsedTime, int idleType);
private:
	void delegateHandling(float elapsedTime, ActionStruct toRender);
public:
	void update(float elapsedTime);
	virtual ActionStruct handleAI(float elapsedTime) = 0;
};