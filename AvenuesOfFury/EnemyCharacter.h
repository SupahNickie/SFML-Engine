#pragma once
#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include <vector>
#include "Character.h"
#include "PlayerCharacter.h"

class PlayerCharacter;
class EnemyCharacter : public Character {
protected:
	PlayerCharacter* focusChar;
	bool deciding = false;
	int timeSinceDecision = 0;
	int timeSinceAttackBegan = 0;
	int timeSinceAttackEnded = 0;
	float varianceSpeed = 0.0f;
	float varianceHealth = 0.0f;
	float varianceAggression = 0.0f;
	float varianceDefensiveness = 0.0f;
	int aggression = 0; // minimum milliseconds before attacking again
	int defense = 0; // minimum milliseconds "reaction time" to player attack
public:
	void update(float elapsedTime, vector<PlayerCharacter*> players);
	void turnToFaceFocusChar();
	void attack(float elapsedTime);
	virtual void handleAI(float elapsedTime, vector<PlayerCharacter*> players) = 0;
};

#endif