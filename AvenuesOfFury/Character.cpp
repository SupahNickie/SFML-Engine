#include "pch.h"
#include "Character.h"
#include "SpriteHolder.h"
#include <iostream>

void Character::flipHorizontally() {
	Graphic::flipHorizontally();
	facingLeft = !facingLeft;
	facingRight = !facingRight;
}

Character::DirectionHeaded Character::stringToDirection(string const& direction) {
	if (direction == "U") return Character::DirectionHeaded::U;
	if (direction == "UR") return Character::DirectionHeaded::UR;
	if (direction == "R") return Character::DirectionHeaded::R;
	if (direction == "DR") return Character::DirectionHeaded::DR;
	if (direction == "D") return Character::DirectionHeaded::D;
	if (direction == "DL") return Character::DirectionHeaded::DL;
	if (direction == "L") return Character::DirectionHeaded::L;
	if (direction == "UL") return Character::DirectionHeaded::UL;
	return Character::DirectionHeaded::NONE;
}

bool Character::hits(Character* otherChar) {
	return this->getPosition().intersects(otherChar->getPosition());
}

void Character::registerHit(int hp) {
	if (spriteState != Globals::ActionType::INJURE) {
		spriteState = Globals::ActionType::INJURE;
		currentAction = "injure";
		currentActionType = INJURE_1;
		resetFrameState();
	}
	health -= hp;
}

void Character::resetFrameState() {
	spriteCycleDown = false;
	currentActionDone = false;
	timeSinceLastFrame = 0;
	timeSinceLastAction = 0;
	currentFrame = SpriteHolder::getStartFramesForAction(spriteName, currentAction, currentActionType);
}

void Character::updateFrameState(float elapsedTime) {
	timeSinceLastAction += elapsedTime * 1000;
	timeSinceLastFrame += elapsedTime * 1000;
	if (timeSinceLastFrame > MS_PER_FRAME) {
		hitRegistered = false;
		int maxFrames = SpriteHolder::getMaxFramesForAction(spriteName, currentAction, currentActionType);
		if (0 == maxFrames) {
			// do nothing, no animation needed
			timeSinceLastFrame = 0;
			return;
		}
		else if (currentFrame >= maxFrames && !spriteCycleDown) {
			--currentFrame;
			spriteCycleDown = true;
			if (!animationCycle[currentAction]) {
				resetFrameState();
				currentActionDone = true;
			}
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