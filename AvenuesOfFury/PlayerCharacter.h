#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <vector>
#include <map>
#include "Character.h"
#include "EnemyCharacter.h"
#include <SFML/Graphics.hpp>

struct PlayerPastPosition {
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
	map<int, Vector2f> pastPositions = {
		{500, Vector2f(0,0)},
		{450, Vector2f(0,0)},
		{400, Vector2f(0,0)},
		{350, Vector2f(0,0)},
		{300, Vector2f(0,0)},
		{250, Vector2f(0,0)},
		{200, Vector2f(0,0)},
		{150, Vector2f(0,0)},
		{100, Vector2f(0,0)},
		{50, Vector2f(0,0)},
		{0, Vector2f(0,0)}
	};
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
	PlayerPastPosition getPastPosition(int time);
};

#endif