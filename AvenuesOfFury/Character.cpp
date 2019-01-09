#include "pch.h"
#include "Character.h"
#include "SpriteHolder.h"
#include "Globals.h"
#include <iostream>

Character::Character() {
	uniqueID = Globals::getAndIncrementIDSerial();

	// Init position data for enemies
	CharacterVelocity cv;
	cv.direction = Graphic::DirectionHeaded::NONE;
	cv.position = position;
	pastPositions = {
		{500, cv},
		{450, cv},
		{400, cv},
		{350, cv},
		{300, cv},
		{250, cv},
		{200, cv},
		{150, cv},
		{100, cv},
		{50, cv},
		{0, cv}
	};
}

void Character::flipHorizontally() {
	Graphic::flipHorizontally();
	facingRight = !facingRight;
}

void Character::disable() {
	disabled = true;
	timeSinceLastAction = 0;
}

Graphic::DirectionHeaded Character::stringToDirection(string const& direction) {
	if (direction == "U") return Graphic::DirectionHeaded::U;
	if (direction == "UR") return Graphic::DirectionHeaded::UR;
	if (direction == "R") return Graphic::DirectionHeaded::R;
	if (direction == "DR") return Graphic::DirectionHeaded::DR;
	if (direction == "D") return Graphic::DirectionHeaded::D;
	if (direction == "DL") return Graphic::DirectionHeaded::DL;
	if (direction == "L") return Graphic::DirectionHeaded::L;
	if (direction == "UL") return Graphic::DirectionHeaded::UL;
	return Graphic::DirectionHeaded::NONE;
}

bool Character::hits(Character* otherChar) {
	return this->getPosition().intersects(otherChar->getPosition());
}

void Character::registerHit(int hp) {
	spriteState = Globals::ActionType::INJURE;
	currentAction = "injure";
	currentActionType = INJURE_1;
	resetFrameState();
	health -= hp;
}

void Character::detectCollisions(vector<Character*> players, vector<Character*> enemies) {
	playersTouching.clear();
	enemiesTouching.clear();
	for_each(players.begin(), players.end(), [&](Character* p) {
		if (p->uniqueID != uniqueID) {
			if (hits(p)) {
				playersTouching.push_back(p);
			}
		}
	});
	for_each(enemies.begin(), enemies.end(), [&](Character* e) { 
		if (e->uniqueID != uniqueID) {
			if (hits(e)) {
				enemiesTouching.push_back(e);
			}
		}
	});
}

void Character::resetFrameState(bool clearAll) {
	if (clearAll) {
		timeSinceLastFrame = 0;
		timeSinceLastAction = 0;
	}
	spriteCycleDown = false;
	currentActionDone = false;
	currentFrame = SpriteHolder::getStartFramesForAction(spriteName, currentAction, currentActionType);
}

void Character::updateFrameState(float elapsedTime, bool prioritizedAction, bool jumping) {
	timeSinceLastAction += elapsedTime * 1000;
	timeSinceLastFrame += elapsedTime * 1000;
	if (timeSinceLastFrame > MS_PER_FRAME) {
		if (spriteName == "skate") {
			cout << "TIME SINCE LAST FRAME : " << timeSinceLastFrame << " CURRENT ACTION " << currentAction << "\n";
		}
		int maxFrames = SpriteHolder::getMaxFramesForAction(spriteName, currentAction, currentActionType);
		if (jumping && handleJumpingAnimation(maxFrames, prioritizedAction)) return;
		handleNormalAnimation(maxFrames);
		timeSinceLastFrame = 0;
	}
}

bool Character::handleJumpingAnimation(int maxFrames, bool attacking) {
	if (timeSinceLastAction > jumpLength) {
		spriteState = Globals::ActionType::JUMP_LAND;
		currentAction = "jump_land";
		currentActionType = JUMP_LAND;
		resetFrameState(false);
		timeSinceLastFrame = 0;
		jumping = false;
		jumpDisabled = true;
		prejumpY = 0.0f;
		return true;
	}

	if (maxFrames == currentFrame) {
		if (attacking) {
			if (spriteState != Globals::ActionType::JUMP_ATTACK) {
				spriteState = Globals::ActionType::JUMP_ATTACK;
				currentAction = "jump_attack";
				currentActionType = JUMP_ATTACK;
				resetFrameState(false);
			}
			// Do nothing, let the last frame always hold until touching the ground or contact made
			timeSinceLastFrame = 0;
			return true;
		}
		switch (spriteState) {
		case Globals::ActionType::JUMP_START:
			spriteState = Globals::ActionType::JUMP_AIR;
			currentAction = "jump_air";
			currentActionType = JUMP_AIR;
			resetFrameState(false);
			break;
		case Globals::ActionType::JUMP_AIR:
			spriteState = Globals::ActionType::JUMP_LAND;
			currentAction = "jump_land";
			currentActionType = JUMP_LAND;
			resetFrameState(false);
			break;
		}

		timeSinceLastFrame = 0;
		return true;
	}

	if (attacking) {
		if (spriteState != Globals::ActionType::JUMP_ATTACK) {
			spriteState = Globals::ActionType::JUMP_ATTACK;
			currentAction = "jump_attack";
			currentActionType = JUMP_ATTACK;
			resetFrameState(false);
		}
	}

	return false;
}

void Character::handleNormalAnimation(int maxFrames) {
	hitRegistered = false;
	jumpDisabled = false;

	if (0 == maxFrames) {
		// do nothing, no animation needed
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
}

CharacterVelocity Character::getVelocity(int time) {
	CharacterVelocity output;
	time == 0 ? output.position = position : output.position = pastPositions[time].position;
	output.direction = directionHeaded;
	return output;
}

void Character::updatePastPositions(float elapsedTime) {
	if (timeSincePastPositionsUpdate > 50) {
		int positionToUpdate = 500;
		int positionToGrab = 450;
		while (positionToUpdate > 0) {
			pastPositions[positionToUpdate] = pastPositions[positionToGrab];
			positionToUpdate = positionToGrab;
			positionToGrab -= 50;
		}
		CharacterVelocity current;
		current.direction = directionHeaded;
		current.position = position;
		if (jumping) current.position.y = prejumpY;
		pastPositions[0] = current;
		timeSincePastPositionsUpdate = 0;
	}
	timeSincePastPositionsUpdate += elapsedTime * 1000;
}

void Character::setAttackState(int attackType) {
	if (!attackDisabled) {
		currentAction = "attack";
		if (currentActionType != attackType) {
			currentActionType = attackType;
			resetFrameState();
		}
		if (spriteState != Globals::ActionType::ATTACK) {
			spriteState = Globals::ActionType::ATTACK;
			resetFrameState();
		}
	}
	if (currentActionDone) attackDisabled = true;
}

void Character::render() {
	SpriteHolder::setSprite(sprite, spriteName, currentAction, currentActionType, currentFrame);
}