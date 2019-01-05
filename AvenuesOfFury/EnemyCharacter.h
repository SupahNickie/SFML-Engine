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
	int varianceSpeed = 0;
	int varianceHealth = 0;
	int varianceReaction = 0;
	int reactionSpeed = 0; // minimum milliseconds "reaction time" to player attack

	int baseDecisionSpeed = 500;
	int maxDecisionSpeed = 5000;
	int varianceDecision = 0;
	int decisionSpeed = 0; // amount of time for character to wait before pausing and "thinking"

	int baseAggression = 100;
	int maxAggression = 400;
	int varianceAggression = 0;
	int aggression = 0; // minimum milliseconds before attacking again

	bool handleDecidingState(float elapsedTime, vector<PlayerCharacter*> players);
	bool basicAttack(float elapsedTime);
	bool checkDecidingState();
	void enterMovingState();
	void moveTowardsFocusChar(float elapsedTime);
	void handleAI(float elapsedTime, vector<PlayerCharacter*> players);
	virtual bool handleAttacking(float elapsedTime) = 0;
	virtual void handleMoving(float elapsedTime) = 0;
private:
	void turnToFaceFocusChar();
	void setDirectionHeaded();
	void recalculateAggression();
	void recalculateDecisionSpeed(bool decisionState);
	void resetStateAfterFinishingAction();
public:
	EnemyCharacter(vector<PlayerCharacter*> players);
	void update(float elapsedTime, vector<PlayerCharacter*> players);
	void attack(float elapsedTime);
};

#endif