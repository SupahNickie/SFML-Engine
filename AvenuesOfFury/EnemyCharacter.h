#pragma once
#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include "Character.h"
#include "PlayerCharacter.h"

class PlayerCharacter;
class EnemyCharacter : public Character {
protected:
	PlayerCharacter* focusChar;
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
public:
	void update(float elapsedTime, PlayerCharacter** players);
	void turnToFaceFocusChar();
	void registerHit(int hp, float elapsedTime);
	virtual void handleAI(float elapsedTime, PlayerCharacter** players) = 0;
};

#endif