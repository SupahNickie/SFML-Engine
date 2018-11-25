#include "pch.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Character.h"
#include "SpriteHolder.h"

void Character::setPosition(Vector2f newPosition) {
	position = newPosition;
}

void Character::flipHorizontally() {
	sprite.scale(-1, 1);
	facingLeft = !facingLeft;
	facingRight = !facingRight;
}

void Character::renderMove(float elapsedTime, int moveType) {
	timeSinceMoveFrame += elapsedTime * 1000;
	if (timeSinceMoveFrame > MS_PER_FRAME) {
		if (moveFrame >= SpriteHolder::getMaxFramesForAction(charName, "move", moveType) && !moveSpriteCycleDown) {
			--moveFrame;
			moveSpriteCycleDown = true;
			if (!animationCycle) moveFrame = 0;
		}
		else if (moveFrame <= 0 && moveSpriteCycleDown) {
			++moveFrame;
			moveSpriteCycleDown = false;
		}
		else {
			moveSpriteCycleDown ? --moveFrame : ++moveFrame;
		}
		timeSinceMoveFrame = 0;
	}

	if (spriteState == SpriteState::IDLE) {
		spriteState = SpriteState::MOVING;
		resetMoveFrame(moveType);
	}
	SpriteHolder::setSprite(sprite, charName, "move", moveType, moveFrame);
}

void Character::renderAttack(float elapsedTime, int attackType) {
	timeSinceAttackFrame += elapsedTime * 1000;
	if (timeSinceAttackFrame > MS_PER_FRAME) {
		if (attackFrame >= SpriteHolder::getMaxFramesForAction(charName, "attack", attackType) && !attackSpriteCycleDown) {
			--attackFrame;
			attackSpriteCycleDown = true;
			if (!animationCycle) attackFrame = 0;
		}
		else if (attackFrame <= 0 && attackSpriteCycleDown) {
			++attackFrame;
			attackSpriteCycleDown = false;
		}
		else {
			attackSpriteCycleDown ? --attackFrame : ++attackFrame;
		}
		timeSinceAttackFrame = 0;
	}
	SpriteHolder::setSprite(sprite, charName, "attack", attackType, attackFrame);
}

void Character::renderIdle(float elapsedTime, int idleType) {
	timeSinceIdleFrame += elapsedTime * 1000;
	if (timeSinceIdleFrame > MS_PER_FRAME) {
		if (idleFrame >= SpriteHolder::getMaxFramesForAction(charName, "idle", idleType) && !idleSpriteCycleDown) {
			--idleFrame;
			idleSpriteCycleDown = true;
			if (!animationCycle) idleFrame = 0;
		}
		else if (idleFrame <= 0 && idleSpriteCycleDown) {
			++idleFrame;
			idleSpriteCycleDown = false;
		}
		else {
			idleSpriteCycleDown ? --idleFrame : ++idleFrame;
		}
		timeSinceIdleFrame = 0;
	}
	SpriteHolder::setSprite(sprite, charName, "idle", idleType, idleFrame);
}

void Character::resetMoveFrame(int moveType) {
	moveFrame = SpriteHolder::getStartFramesForAction(charName, "move", moveType);
}

void Character::resetAttackFrame(int attackType) {
	attackFrame = SpriteHolder::getStartFramesForAction(charName, "attack", attackType);
}

void Character::resetIdleFrame(int idleType) {
	idleFrame = SpriteHolder::getStartFramesForAction(charName, "attack", idleType);
}

void Character::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}