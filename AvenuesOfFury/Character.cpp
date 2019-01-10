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

	// Init array of direction presses remembered
	pastDirectionsPressed = new Graphic::DirectionHeaded[5];
	pastDirectionsPressed[0] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[1] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[2] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[3] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[4] = Graphic::DirectionHeaded::NONE;
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

string Character::directionToString(Graphic::DirectionHeaded direction) {
	if (direction == Graphic::DirectionHeaded::U) return "U";
	if (direction == Graphic::DirectionHeaded::UR) return "UR";
	if (direction == Graphic::DirectionHeaded::R) return "R";
	if (direction == Graphic::DirectionHeaded::DR) return "DR";
	if (direction == Graphic::DirectionHeaded::D) return "D";
	if (direction == Graphic::DirectionHeaded::DL) return "DL";
	if (direction == Graphic::DirectionHeaded::L) return "L";
	if (direction == Graphic::DirectionHeaded::UL) return "UL";
	return "NONE";
}

bool Character::hits(Character* otherChar) {
	return this->getPosition().intersects(otherChar->getPosition());
}

void Character::registerHit(int hp, string const& attacker, unsigned short int frame) {
	auto it = hitsRegistered.find(attacker);
	if (it != hitsRegistered.end()) {
		if (it->second.frame == frame && it->second.timeSinceHitRegistered < MS_PER_FRAME) return;
	}
	HitRecord output;
	output.frame = frame;
	output.timeSinceHitRegistered = 0;
	hitsRegistered[attacker] = output;

	spriteState = Globals::ActionType::INJURE;
	currentAction = "injure";
	currentActionType = INJURE_1;
	resetFrameState();
	health -= hp;
}

void Character::advanceHitRecords(float elapsedTime) {
	for (auto it = hitsRegistered.begin(); it != hitsRegistered.end(); /* no incrementer */ ) {
		it->second.timeSinceHitRegistered += elapsedTime * 1000;
		if (it->second.timeSinceHitRegistered > MS_PER_FRAME) {
			it = hitsRegistered.erase(it);
		}
		else {
			++it;
		}
	}
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
	if (clearAll) timeSinceLastAction = 0;
	timeSinceLastFrame = 0;
	spriteCycleDown = false;
	currentActionDone = false;
	currentFrame = SpriteHolder::getStartFramesForAction(spriteName, currentAction, currentActionType);
}

void Character::updateFrameState(float elapsedTime, bool prioritizedAction, bool jumping) {
	timeSinceLastAction += elapsedTime * 1000;
	timeSinceLastFrame += elapsedTime * 1000;
	if (timeSinceLastFrame > MS_PER_FRAME) {
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

void Character::insertAndShiftPastDirectionsPressed(Graphic::DirectionHeaded direction) {
	directionHeaded = direction;
	memmove(&pastDirectionsPressed[0], &pastDirectionsPressed[1], (size_t)4 * sizeof(pastDirectionsPressed[0]));
	pastDirectionsPressed[4] = directionHeaded;
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
		insertAndShiftPastDirectionsPressed(Graphic::DirectionHeaded::NONE);
	}
	if (currentActionDone) attackDisabled = true;
}

void Character::render() {
	SpriteHolder::setSprite(sprite, spriteName, currentAction, currentActionType, currentFrame);
}