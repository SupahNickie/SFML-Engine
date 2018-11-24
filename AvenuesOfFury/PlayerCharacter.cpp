#include "pch.h"
#include "PlayerCharacter.h"
#include "SpriteHolder.h"

void PlayerCharacter::update(float elapsedTime) {
	if (primaryAttackPressed) {
		handleAttack(elapsedTime, ATTACK_1);
	}
	else if (secondaryAttackPressed) {
		handleAttack(elapsedTime, ATTACK_2);
	}
	else if (spriteState == SpriteState::ATTACKING && !primaryAttackPressed && !secondaryAttackPressed) {
		resetIdleFrame(IDLE_1);
		spriteState = SpriteState::IDLE;
		handleIdle(elapsedTime, IDLE_1);
		resetMoveFrame(MOVE_1);
		resetAttackFrame(ATTACK_1);
		timeSinceAttackFrame = 0;
		attackDisabled = false;
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		handleIdle(elapsedTime, IDLE_1);
	}

	if (rightPressed && !leftPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		handleMove(elapsedTime, MOVE_1);
	}

	if (leftPressed && !rightPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		handleMove(elapsedTime, MOVE_1);
	}

	if (upPressed && !downPressed) {
		position.y -= speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime, MOVE_1);
	}

	if (downPressed && !upPressed) {
		position.y += speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime, MOVE_1);
	}

	sprite.setPosition(position);
}

void PlayerCharacter::handleMove(float elapsedTime, int moveType) {
	renderMove(elapsedTime, moveType);
}

void PlayerCharacter::handleAttack(float elapsedTime, int attackType) {
	if (attackDisabled) return;

	renderAttack(elapsedTime, attackType);

	if (attackFrame >= SpriteHolder::getAttackTypeMaxFrames(charName)[attackType]) {
		timeSinceHandlingLastAttackFrame += elapsedTime * 1000;
		if (timeSinceHandlingLastAttackFrame >= MS_PER_FRAME) {
			timeSinceAttackFrame = 0;
			timeSinceHandlingLastAttackFrame = 0;
			resetAttackFrame(SpriteHolder::getAttackTypeStartFrames(charName)[attackType]);

			primaryAttackPressed = false;
			secondaryAttackPressed = false;
			attackDisabled = true;

			spriteState = SpriteState::IDLE;
			resetIdleFrame(IDLE_1);
			handleIdle(elapsedTime, IDLE_1);
		}
	}
}

void PlayerCharacter::handleIdle(float elapsedTime, int idleType) {
	renderIdle(elapsedTime, idleType);
}