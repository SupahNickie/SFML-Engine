#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <vector>
#include <map>
#include "Character.h"
#include "EnemyCharacter.h"
#include <SFML/Graphics.hpp>

struct PlayerVelocity {
	Vector2f position;
	Character::DirectionHeaded direction;
};

class EnemyCharacter;
class PlayerCharacter : public Character {
protected:
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool primaryAttackPressed;
	bool secondaryAttackPressed;
	bool inputsDisabled = false;
	int timeSincePastPositionsUpdate = 0;
	int timeSinceLastDirectionPress = 0;
private:
	map<int, PlayerVelocity> pastPositions;
	Character::DirectionHeaded* pastDirectionsPressed;

	void updatePastPositions(float elapsedTime);
	void setDirectionHeaded();
	void hitEnemies(float elapsedTime, vector<EnemyCharacter*> enemies);
	void setMoveState(float elapsedTime);
	void setIdleState(float elapsedTime);
	void setAttackState(float elapsedTime, int attackType);
public:
	PlayerCharacter();
	~PlayerCharacter();
	virtual void handleInput() = 0;
	void update(float elapsedTime, vector<EnemyCharacter*> enemies);
	void disableInputs();
	PlayerVelocity getVelocity(int time);
};

#endif