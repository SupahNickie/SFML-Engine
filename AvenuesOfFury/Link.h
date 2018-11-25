#pragma once
#include "PlayerCharacter.h"

class Link : public PlayerCharacter {
public:
	Link();
	void handleInput();
	void setSecondPlayer();
};