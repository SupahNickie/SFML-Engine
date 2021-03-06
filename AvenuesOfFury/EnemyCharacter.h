#pragma once
#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include <vector>
#include "Character.h"
#include "PlayerCharacter.h"

class Character;
class EnemyCharacter : public Character {
public:
	EnemyCharacter(vector<Character*> players);
	void update(float elapsedTime, vector<Character*> players, vector<Character*> enemies);
	void calculateAttack(float elapsedTime);
protected:
	Vector2f target;
	bool deciding = false;
	bool jumpAttacking = false;
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

	bool checkDecidingState();
	bool handleDecidingState(float elapsedTime, vector<Character*> players);
	void moveTowardsFocusChar(float elapsedTime);
	void predictFocusCharLocation(float elapsedTime);
	void handleAI(float elapsedTime, vector<Character*> players);
	bool attack(float elapsedTime, int actionType);
	bool jumpAttack(float elapsedTime);
	bool grabAttack(float elapsedTime);
	virtual bool handleAttacking(float elapsedTime) = 0;
	virtual void handleMoving(float elapsedTime) = 0;
private:
	void setIdleState(float elapsedTime);
	void setMoveState();
	void turnToFaceFocusChar();
	void setDirectionHeaded();
	void recalculateAggression();
	void recalculateDecisionSpeed(bool decisionState);
	void resetStateAfterFinishingAction();
	void handleFallDamage(float elapsedTime);
	bool handleDisabledState(float elapsedTime);
	void handleJump(float elapsedTime);
	void handleGrab(float elapsedTime);
	void moveTowardsTarget(float elapsedTime);
	void handleRunAttackHorizontal(float elapsedTime);
};

#endif