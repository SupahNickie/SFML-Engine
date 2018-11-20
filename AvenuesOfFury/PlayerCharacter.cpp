#include "pch.h"
#include "PlayerCharacter.h"

using namespace sf;

void PlayerCharacter::update(float elapsedTime) {
	if (attackPressed) {
		handleAttack(elapsedTime);
	}
	else if (spriteState == SpriteState::ATTACKING && !attackPressed) {
		setIdleSprite();
		spriteState = SpriteState::IDLE;
		attack1Frame = 0;
		idle1Frame = 0;
		timeSinceAttack1Frame = 0;
		attackDisabled = false;
	}

	if (rightPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		handleMove(elapsedTime);
	}
	
	if (leftPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		handleMove(elapsedTime);
	}

	if (upPressed) {
		position.y -= speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime);
	}

	if (downPressed) {
		position.y += speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime);
	}

	sprite.setPosition(position);
}

void PlayerCharacter::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}

void PlayerCharacter::flipHorizontally() {
	sprite.scale(-1, 1);
	facingLeft = !facingLeft;
	facingRight = !facingRight;
}