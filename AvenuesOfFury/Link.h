#pragma once
#include "PlayerCharacter.h"

class Link : public PlayerCharacter {
private:
	const Vector2i IDLE_SPRITE_ORIGIN = Vector2i(395, 0);
	const Vector2i IDLE_SPRITE_BOUNDS = Vector2i(142, 178);

	void resetMoveFrame(int moveType);
public:
	Link();
	void virtual handleInput();
	void setIdleSprite();
};