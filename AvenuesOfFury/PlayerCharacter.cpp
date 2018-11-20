#include "pch.h"
#include "PlayerCharacter.h"

using namespace sf;

void PlayerCharacter::update(float elapsedTime) {
	if (attackPressed) {
		handleAttack(elapsedTime, ATTACK_NORMAL);
	}
	else if (spriteState == SpriteState::ATTACKING && !attackPressed) {
		setIdleSprite();
		spriteState = SpriteState::IDLE;
		attackFrame = 0;
		idleFrame = 0;
		timeSinceAttackFrame = 0;
		attackDisabled = false;
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		setIdleSprite();
	}

	if (rightPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		handleMove(elapsedTime, MOVE_WALK);
	}
	
	if (leftPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		handleMove(elapsedTime, MOVE_WALK);
	}

	if (upPressed) {
		position.y -= speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime, MOVE_WALK);
	}

	if (downPressed) {
		position.y += speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime, MOVE_WALK);
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

void PlayerCharacter::resetAttackFrame() {
	attackFrame = 0;
}

void PlayerCharacter::handleAttack(float elapsedTime, int attackType) {
	if (attackDisabled) return;
	timeSinceAttackFrame += elapsedTime * 1000;
	if (timeSinceAttackFrame > MS_PER_FRAME) {
		++attackFrame;
		timeSinceAttackFrame = 0;
	}

	if (attackFrame > attackTypeMaxFrames[attackType]) {
		setIdleSprite();
		spriteState = SpriteState::IDLE;
		attackFrame = 0;
		idleFrame = 0;
		timeSinceAttackFrame = 0;
		attackPressed = false;
		attackDisabled = true;
	}
	else {
		setAttackSprite(attackType);
	}
}

void PlayerCharacter::handleMove(float elapsedTime, int moveType) {
	timeSinceMoveFrame += elapsedTime * 1000;
	if (timeSinceMoveFrame > MS_PER_FRAME) {
		moveSpriteCycleDown ? --moveFrame : ++moveFrame;
		timeSinceMoveFrame = 0;
	}

	if (moveFrame > moveTypeMaxFrames[moveType] && !moveSpriteCycleDown) {
		moveSpriteCycleDown = true;
		--moveFrame;
		--moveFrame;
	}
	else if (moveFrame < 0 && moveSpriteCycleDown) {
		moveSpriteCycleDown = false;
		++moveFrame;
		++moveFrame;
	}

	setMoveSprite(moveType);
}

void PlayerCharacter::setAttackSprite(int attackType) {
	sprite.setTextureRect(
		IntRect(
			attackSpriteOrigins[attackType][attackFrame].x,
			attackSpriteOrigins[attackType][attackFrame].y,
			attackSpriteBounds[attackType][attackFrame].x,
			attackSpriteBounds[attackType][attackFrame].y
		)
	);
	sprite.setOrigin(
		attackSpriteBounds[attackType][attackFrame].x / 2.0f,
		attackSpriteBounds[attackType][attackFrame].y / 2.0f
	);
}

void PlayerCharacter::setMoveSprite(int moveType) {
	if (spriteState == SpriteState::IDLE) {
		spriteState = SpriteState::MOVING;
		resetMoveFrame(moveType);
	}

	sprite.setTextureRect(
		IntRect(
			moveSpriteOrigins[moveType][moveFrame].x,
			moveSpriteOrigins[moveType][moveFrame].y,
			moveSpriteBounds[moveType][moveFrame].x,
			moveSpriteBounds[moveType][moveFrame].y
		)
	);
	sprite.setOrigin(
		moveSpriteBounds[moveType][moveFrame].x / 2.0f,
		moveSpriteBounds[moveType][moveFrame].y / 2.0f
	);
}