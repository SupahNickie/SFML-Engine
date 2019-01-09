#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <vector>
#include <map>
#include "Character.h"
#include "EnemyCharacter.h"
#include <SFML/Graphics.hpp>

class PlayerCharacter : public Character {
protected:
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool primaryAttackPressed;
	bool secondaryAttackPressed;
	bool jumpPressed;
	int timeSinceLastDirectionPress = 0;
private:
	Graphic::DirectionHeaded* pastDirectionsPressed;

	void setDirectionHeaded();
	void handleJump(float elapsedTime, bool attacking);
	void handleNextJumpFrame(float elapsedTime, bool attacking);
	void hitCharacters(float elapsedTime);
	void setMoveState(float elapsedTime);
	void setIdleState(float elapsedTime);
public:
	PlayerCharacter();
	~PlayerCharacter();
	virtual void handleInput() = 0;
	void update(float elapsedTime, vector<Character*> players, vector<Character*> enemies);
};

#endif