#pragma once
#include "PlayerCharacter.h"

class Skate : public PlayerCharacter {
public:
	Skate();
	void handleInput();
protected:
	void handleSpecialAttack();
};