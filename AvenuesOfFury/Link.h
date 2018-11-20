#pragma once
#include "PlayerCharacter.h"

class Link : public PlayerCharacter {
private:
	const Vector2i IDLE_SPRITE_ORIGIN = Vector2i(395, 0);
	const Vector2i IDLE_SPRITE_BOUNDS = Vector2i(142, 178);

	void initAttackSprites();
	void initMoveSprites();
	void setAttack1Sprite();
	void setMove1Sprite();
public:
	Link();
	void virtual handleInput();
	void handleAttack(float elapsedTime);
	void handleMove(float elapsedTime);
	void setIdleSprite();
};