#pragma once
#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include "Character.h"
#include "PlayerCharacter.h"

class PlayerCharacter;
class EnemyCharacter : public Character {
protected:
	int timeSinceDecision = 0;
	int timeSinceAttackBegan = 0;
	int timeSinceAttackEnded = 0;
	float varianceSpeed = 0.0f;
	float varianceHealth = 0.0f;
	float varianceAggression = 0.0f;
	float varianceDefensiveness = 0.0f;
	int health = 0;
	int aggression = 0; // minimum milliseconds before attacking again
	int defense = 0; // minimum milliseconds "reaction time" to player attack

	void handleMove(float elapsedTime, int moveType);
	void handleAttack(float elapsedTime, int attackType);
	void handleIdle(float elapsedTime, int idleType);
private:
	void delegateHandling(float elapsedTime, ActionStruct toRender);
public:
	void update(float elapsedTime, PlayerCharacter** players);
	void registerHit(int hp);
	virtual ActionStruct handleAI(float elapsedTime, PlayerCharacter** players) = 0;
};

#endif