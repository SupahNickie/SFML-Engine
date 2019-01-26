#pragma once
#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <vector>
#include <map>
#include "Character.h"
#include "EnemyCharacter.h"
#include <SFML/Graphics.hpp>

class PlayerCharacter : public Character {
public:
	~PlayerCharacter();
	void update(float elapsedTime, vector<Character*> players, vector<Character*> enemies);
	virtual void handleInput() = 0;
protected:
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool primaryAttackPressed;
	bool secondaryAttackPressed;
	bool jumpPressed;
	bool updateHandled = false;
	bool resetToIdle = true;
	int timeSinceLastDirectionPress = 0;
private:
	void setDirectionHeaded();
	string determineAttackingIntention();
	void hitCharacters(float elapsedTime);
	void setMoveState(float elapsedTime);
	void setIdleState(float elapsedTime);
	void handleRunAttackHorizontal(float elapsedTime);
};

#endif