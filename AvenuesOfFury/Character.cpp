#include "pch.h"
#include "Character.h"
#include "SpriteHolder.h"

void Character::flipHorizontally() {
	Graphic::flipHorizontally();
	facingLeft = !facingLeft;
	facingRight = !facingRight;
}

bool Character::hits(Character* otherChar) {
	if (this->spriteState != Globals::ActionType::ATTACK) return false;
	return this->getPosition().intersects(otherChar->getPosition());
}

void Character::resetFrameState() {
	spriteCycleDown = false;
	currentActionDone = false;
	timeSinceLastFrame = 0;
	currentFrame = SpriteHolder::getStartFramesForAction(spriteName, currentAction, currentActionType);
}

void Character::updateFrameState(float elapsedTime) {
	timeSinceLastFrame += elapsedTime * 1000;
	if (timeSinceLastFrame > MS_PER_FRAME) {
		if (currentFrame >= SpriteHolder::getMaxFramesForAction(spriteName, currentAction, currentActionType) && !spriteCycleDown) {
			--currentFrame;
			spriteCycleDown = true;
			if (!animationCycle) resetFrameState();
		}
		else if (currentFrame <= 0 && spriteCycleDown) {
			++currentFrame;
			spriteCycleDown = false;
		}
		else {
			spriteCycleDown ? --currentFrame : ++currentFrame;
		}
		timeSinceLastFrame = 0;
	}
}

void Character::render() {
	SpriteHolder::setSprite(sprite, spriteName, currentAction, currentActionType, currentFrame);
}