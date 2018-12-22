#include "pch.h"
#include <iostream>
#include "Character.h"
#include "SpriteHolder.h"

void Character::flipHorizontally() {
	Graphic::flipHorizontally();
	facingLeft = !facingLeft;
	facingRight = !facingRight;
}

bool Character::hits(Character* otherChar) {
	if (this->spriteState != SpriteState::ATTACKING) return false;
	return this->getPosition().intersects(otherChar->getPosition());
}

void Character::renderMove(float elapsedTime, int moveType) {
	timeSinceMoveFrame += elapsedTime * 1000;
	if (timeSinceMoveFrame > MS_PER_FRAME) {
		if (moveFrame >= SpriteHolder::getMaxFramesForAction(spriteName, "move", moveType) && !moveSpriteCycleDown) {
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
	SpriteHolder::setSprite(sprite, spriteName, "move", moveType, moveFrame);
}

void Character::renderAttack(float elapsedTime, int attackType) {
	timeSinceAttackFrame += elapsedTime * 1000;
	if (timeSinceAttackFrame > MS_PER_FRAME) {
		if (attackFrame >= SpriteHolder::getMaxFramesForAction(spriteName, "attack", attackType) && !attackSpriteCycleDown) {
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
	SpriteHolder::setSprite(sprite, spriteName, "attack", attackType, attackFrame);
}

void Character::renderIdle(float elapsedTime, int idleType) {
	timeSinceIdleFrame += elapsedTime * 1000;
	if (timeSinceIdleFrame > MS_PER_FRAME) {
		if (idleFrame >= SpriteHolder::getMaxFramesForAction(spriteName, "idle", idleType) && !idleSpriteCycleDown) {
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
	SpriteHolder::setSprite(sprite, spriteName, "idle", idleType, idleFrame);
}

void Character::resetMoveFrame(int moveType) {
	moveFrame = SpriteHolder::getStartFramesForAction(spriteName, "move", moveType);
}

void Character::resetAttackFrame(int attackType) {
	attackFrame = SpriteHolder::getStartFramesForAction(spriteName, "attack", attackType);
}

void Character::resetIdleFrame(int idleType) {
	idleFrame = SpriteHolder::getStartFramesForAction(spriteName, "attack", idleType);
}