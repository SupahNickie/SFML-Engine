#pragma once
#include "PlayerCharacter.h"

class Skate : public PlayerCharacter {
public:
	Skate();
	void virtual handleInput();
	void setSecondPlayer();
};