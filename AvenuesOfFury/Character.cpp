#include "pch.h"
#include "Character.h"
#include "SpriteHolder.h"
#include "Globals.h"
#include <assert.h>
#include <iostream>

Character::Character() {
	uniqueID = Globals::getAndIncrementIDSerial();

	// Init position data for enemies
	CharacterVelocity cv;
	cv.direction = DirectionHeaded::NONE;
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
	pastDirectionsPressed = new DirectionHeaded[5];
	pastDirectionsPressed[0] = DirectionHeaded::NONE;
	pastDirectionsPressed[1] = DirectionHeaded::NONE;
	pastDirectionsPressed[2] = DirectionHeaded::NONE;
	pastDirectionsPressed[3] = DirectionHeaded::NONE;
	pastDirectionsPressed[4] = DirectionHeaded::NONE;
}

void Character::flipHorizontally() {
	Graphic::flipHorizontally();
	facingRight = !facingRight;
}

bool Character::isFacingRight() {
	return facingRight;
}

bool Character::isInvincible() {
	return (
		spriteState == Globals::ActionType::FALL ||
		spriteState == Globals::ActionType::RISE ||
		spriteState == Globals::ActionType::INJURE
	);
}

void Character::disable(int timeToDisable) {
	disabled = true;
	timeToBeDisabled = timeToDisable;
	timeSinceLastAction = 0;
}

void Character::hold(bool state, Character* attacker) {
	if (state) {
		spriteState = Globals::ActionType::HELD;
		currentAction = "held";
		currentActionType = HELD;
		resetFrameState();
		held = true;
		attackedBy = attacker;
	}
	else {
		held = false;
	}
}

Character::DirectionHeaded Character::stringToDirection(string const& direction) {
	if (direction == "U") return DirectionHeaded::U;
	if (direction == "UR") return DirectionHeaded::UR;
	if (direction == "R") return DirectionHeaded::R;
	if (direction == "DR") return DirectionHeaded::DR;
	if (direction == "D") return DirectionHeaded::D;
	if (direction == "DL") return DirectionHeaded::DL;
	if (direction == "L") return DirectionHeaded::L;
	if (direction == "UL") return DirectionHeaded::UL;
	return DirectionHeaded::NONE;
}

string Character::directionToString(DirectionHeaded direction) {
	if (direction == DirectionHeaded::U) return "U";
	if (direction == DirectionHeaded::UR) return "UR";
	if (direction == DirectionHeaded::R) return "R";
	if (direction == DirectionHeaded::DR) return "DR";
	if (direction == DirectionHeaded::D) return "D";
	if (direction == DirectionHeaded::DL) return "DL";
	if (direction == DirectionHeaded::L) return "L";
	if (direction == DirectionHeaded::UL) return "UL";
	return "NONE";
}

bool Character::hits(Character* otherChar) {
	return this->getPosition().intersects(otherChar->getPosition());
}

void Character::registerHit(int hp, string const& attacker, unsigned short int frame, AttackInfo info) {
	if (fallstatus != FallStep::NONE) return; // Can't kick dudes when they're down
	auto it = hitsRegistered.find(attacker);
	if (it != hitsRegistered.end()) {
		if (it->second.frame == frame && it->second.timeSinceHitRegistered < MS_PER_FRAME) return;
	}
	HitRecord output;
	output.frame = frame;
	output.timeSinceHitRegistered = 0;
	hitsRegistered[attacker] = output;

	spriteState = info.actionType;
	currentAction = info.action;
	currentActionType = info.injuryType;
	fallstatus = info.fallstatus;
	fallY = info.fallY;
	fallDirection = info.fallDirection;
	resetFrameState();

	jumping = false;
	health -= hp;
}

CharacterVelocity Character::getVelocity(int time) {
	CharacterVelocity output;
	time == 0 ? output.position = position : output.position = pastPositions[time].position;
	output.direction = directionHeaded;
	return output;
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
				determineAndSetGrabChar(p);
			}
		}
	});
	for_each(enemies.begin(), enemies.end(), [&](Character* e) { 
		if (e->uniqueID != uniqueID) {
			if (hits(e)) {
				enemiesTouching.push_back(e);
				determineAndSetGrabChar(e);
			}
		}
	});
	if (playersTouching.empty() && enemiesTouching.empty()) resetGrab();
}

bool Character::onSameVerticalPlane(float targetY) {
	if (prejumpY != 0.0f) {
		return (abs(targetY - prejumpY) < (.03f * Globals::getResolution().x) &&
			abs(targetY - position.y) < (.062500f * Globals::getResolution().x));
	}
	else {
		return abs(targetY - position.y) < (.03f * Globals::getResolution().x);
	}
}

void Character::resetFrameState(bool clearAll) {
	if (clearAll) timeSinceLastAction = 0;
	timeSinceLastFrame = 0;
	spriteCycleDown = false;
	currentActionDone = false;
	currentFrame = SpriteHolder::getStartFramesForAction(spriteName, currentAction, currentActionType);
}

void Character::updateFrameState(float elapsedTime, string info) {
	timeSinceLastAction += elapsedTime * 1000;
	timeSinceLastFrame += elapsedTime * 1000;
	if (timeSinceLastFrame > MS_PER_FRAME) {
		int maxFrames = SpriteHolder::getMaxFramesForAction(spriteName, currentAction, currentActionType);
		if (handleHeldAnimation(maxFrames, info, elapsedTime)) return;
		if (handleGrabbingAnimation(maxFrames, info, elapsedTime)) return;
		if (handleRunningAnimation(maxFrames, info, elapsedTime)) return;
		if (handleJumpingAnimation(maxFrames, info, elapsedTime)) return;
		if (handleFallingAnimation(maxFrames, info, elapsedTime)) return;
		if (handleInjureAnimation(maxFrames, info, elapsedTime)) return;
		handleNormalAnimation(maxFrames);
		timeSinceLastFrame = 0;
	}
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

void Character::insertAndShiftPastDirectionsPressed(DirectionHeaded direction) {
	directionHeaded = direction;
	memmove(&pastDirectionsPressed[0], &pastDirectionsPressed[1], (size_t)4 * sizeof(pastDirectionsPressed[0]));
	pastDirectionsPressed[4] = directionHeaded;
}

void Character::setAttackState(string const& action, int attackType, bool resetFrame) {
	if (!attackDisabled) {
		bool changed = false;
		currentAction = action;
		if (currentAction == "attack") {
			if (spriteState != Globals::ActionType::ATTACK) {
				spriteState = Globals::ActionType::ATTACK;
				changed = true;
			}
		}
		if (currentAction == "jump_attack") {
			if (spriteState != Globals::ActionType::JUMP_ATTACK) {
				spriteState = Globals::ActionType::JUMP_ATTACK;
				changed = true;
			}
		}
		if (currentAction == "run_attack") {
			if (spriteState != Globals::ActionType::RUN_ATTACK) {
				spriteState = Globals::ActionType::RUN_ATTACK;
				changed = true;
				speedY = baseSpeedY;
				prejumpY = position.y;
				position.y -= 0.0042f * Globals::getResolution().x;
				runAttacking = true;
			}
		}
		if (currentAction == "grab") {
			if (spriteState != Globals::ActionType::GRAB) {
				spriteState = Globals::ActionType::GRAB;
				changed = true;
			}
		}
		if (currentAction == "grab_attack_head") {
			if (spriteState != Globals::ActionType::GRAB_ATTACK_HEAD) {
				spriteState = Globals::ActionType::GRAB_ATTACK_HEAD;
				++grabHits;
				changed = true;
			}
		}
		if (currentAction == "grab_attack_body") {
			if (spriteState != Globals::ActionType::GRAB_ATTACK_BODY) {
				spriteState = Globals::ActionType::GRAB_ATTACK_BODY;
				++grabHits;
				changed = true;
			}
		}
		if (currentAction == "throw") {
			if (spriteState != Globals::ActionType::THROW) {
				spriteState = Globals::ActionType::THROW;
				changed = true;
			}
		}

		if (currentActionType != attackType) {
			currentActionType = attackType;
			changed = true;
		}
		if (changed) resetFrameState(resetFrame);
		insertAndShiftPastDirectionsPressed(DirectionHeaded::NONE);
	}
	if (currentActionDone) attackDisabled = true;
}

void Character::setJumpState(float elapsedTime, bool moveLeft, bool moveRight) {
	if (!Globals::isJumpingState(spriteState)) {
		spriteState = Globals::ActionType::JUMP_START;
		currentAction = "jump_start";
		currentActionType = JUMP_START;
		resetFrameState();
		jumping = true;
		running = false;
		prejumpY = position.y;
		position.y -= 0.0042f * Globals::getResolution().x;
		insertAndShiftPastDirectionsPressed(DirectionHeaded::NONE);
		speedY = baseSpeedY;
	}

	speedY += (.2 * gravity * (elapsedTime * 1000));
	if (prejumpY > position.y) position.y += speedY;
	if (moveLeft) position.x -= speed * elapsedTime;
	if (moveRight) position.x += speed * elapsedTime;
}

AttackInfo Character::generateAttackInfo(bool longStun, Character* otherChar) {
	AttackInfo output;
	if (grabHits > 2 ||
		(spriteState == Globals::ActionType::ATTACK && currentActionType == SPECIAL_ATTACK) ||
		spriteState == Globals::ActionType::JUMP_ATTACK ||
		spriteState == Globals::ActionType::RUN_ATTACK ||
		spriteState == Globals::ActionType::THROW ||
		spriteState == Globals::ActionType::FALL
		) {
		output.action = "fall";
		longStun && !isPlayer ? output.timeToDisable = STUN_LENGTH * 15 : output.timeToDisable = STUN_LENGTH * 5;
		output.fallstatus = FallStep::START_FALL;
		output.fallY = otherChar->getCenter().y + (otherChar->getPosition().height / 4);
		output.fallDirection = getDirectionOfCollision(otherChar);
		output.injuryType = FALL;
	}
	else if (spriteState == Globals::ActionType::ATTACK ||
		spriteState == Globals::ActionType::GRAB_ATTACK_HEAD ||
		spriteState == Globals::ActionType::GRAB_ATTACK_BODY
		) {
		output.action = "injure";
		output.timeToDisable = STUN_LENGTH;
		output.fallstatus = FallStep::NONE;
		output.fallY = 0.0f;
		output.fallDirection = DirectionHeaded::NONE;
		output.injuryType = currentActionType;
	}
	output.actionType = Globals::actionStringToEnum(output.action);
	return output;
}

void Character::render() {
	SpriteHolder::setSprite(sprite, spriteName, currentAction, currentActionType, currentFrame);
}

bool Character::handleHeldAnimation(int maxFrames, string info, float elapsedTime) {
	if (held) return true;
	return false;
}

bool Character::handleGrabbingAnimation(int maxFrames, string info, float elapsedTime) {
	if (grabbing) {
		if (currentFrame == maxFrames) {
			if (currentAction == "throw" || grabHits > 2) {
				setIdleState(elapsedTime);
				resetFrameState();
				resetGrab();
				return false;
			}
			if (currentAction != "grab") {
				setAttackState("grab", GRAB);
				grabbedChar->hold(true, this);
			}
			return true;
		}
	}
	return false;
}

bool Character::handleRunningAnimation(int maxFrames, string info, float elapsedTime) {
	if (running) {
		if (runAttacking && ((runAttackJumps && prejumpY <= position.y) ||
			(!runAttackJumps && currentFrame == maxFrames))
			) {
			disabled = true;
			timeToBeDisabled = MS_PER_FRAME * SpriteHolder::getMaxFramesForAction(spriteName, "run_attack_land", RUN_ATTACK_LAND);
			spriteState = Globals::ActionType::RUN_ATTACK_LAND;
			currentAction = "run_attack_land";
			currentActionType = RUN_ATTACK_LAND;
			runAttacking = false;
			running = false;
			resetFrameState();
			return true;
		}

		if (runAttacking) {
			handleRunAttackHorizontal(elapsedTime);
		}

		if (runAttacking && runAttackJumps) {
			speedY += (.6 * gravity * (elapsedTime * 1000));
			position.y += speedY;
			if (maxFrames == currentFrame) return true;
		}
	}
	return false;
}

bool Character::handleJumpingAnimation(int maxFrames, string info, float elapsedTime) {
	if (jumping) {
		if (prejumpY <= position.y) {
			spriteState = Globals::ActionType::JUMP_LAND;
			currentAction = "jump_land";
			currentActionType = JUMP_LAND;
			resetFrameState(false);
			jumping = false;
			jumpDisabled = true;
			prejumpY = 0.0f;
			currentActionDone = true;
			return true;
		}

		if (maxFrames == currentFrame) {
			if (!info.empty() && info != "jump") {
				if (spriteState != Globals::ActionType::JUMP_ATTACK) {
					setAttackState("jump_attack", JUMP_ATTACK, false);
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

		if (!info.empty() && info != "jump") {
			setAttackState("jump_attack", JUMP_ATTACK, false);
		}
	}
	return false;
}

bool Character::handleFallingAnimation(int maxFrames, string info, float elapsedTime) {
	if (fallstatus != FallStep::NONE) {
		if (fallstatus == FallStep::START_FALL) {
			speedY = -0.004f * Globals::getResolution().y;
			fallstatus = FallStep::KNOCK_DOWN;
			if (fallDirection == DirectionHeaded::UR || fallDirection == DirectionHeaded::UL || fallDirection == DirectionHeaded::U) {
				fallY = position.y - (0.3f * Globals::getResolution().y);
				speedY = -0.010f * Globals::getResolution().y;
			}
			if (fallDirection == DirectionHeaded::DR || fallDirection == DirectionHeaded::DL || fallDirection == DirectionHeaded::D) {
				fallY = position.y + (0.3f * Globals::getResolution().y);
			}
			if (fallDirection == DirectionHeaded::L || fallDirection == DirectionHeaded::R) {
				speedY = -0.010f * Globals::getResolution().y;
			}
			previousY = position.y;
			pastZenith = false;
		}

		if (fallstatus == FallStep::KNOCK_DOWN) {
			if (fallDirection == DirectionHeaded::L || fallDirection == DirectionHeaded::UL || fallDirection == DirectionHeaded::DL) position.x -= 4 * baseSpeed * elapsedTime;
			if (fallDirection == DirectionHeaded::R || fallDirection == DirectionHeaded::UR || fallDirection == DirectionHeaded::DR) position.x += 4 * baseSpeed * elapsedTime;
		}

		speedY += (gravity * (elapsedTime * 1000));
		if (!pastZenith || fallY > position.y) {
			previousY = position.y;
			position.y += speedY;
			if (previousY < position.y) pastZenith = true;
			return true;
		}

		if (currentFrame == maxFrames) {
			if (fallstatus == FallStep::KNOCK_DOWN) {
				if (fallY > position.y) return true;
				resetFrameState();
				fallstatus = FallStep::BOUNCE_UP;
			}
			if (fallstatus == FallStep::BOUNCE_UP) {
				// Time remaining is less than amount of frames their rising animation is
				if ((timeToBeDisabled - timeSinceLastAction) > (MS_PER_FRAME * (SpriteHolder::getMaxFramesForAction(spriteName, "rise", RISE) + 1))) return true;
				spriteState = Globals::ActionType::RISE;
				currentAction = "rise";
				currentActionType = RISE;
				fallstatus = FallStep::NONE;
				attackedBy = nullptr;
				resetFrameState(false);
				return true;
			}
		}
	}
	return false;
}

bool Character::handleInjureAnimation(int maxFrames, string info, float elapsedTime) {
	if (spriteState == Globals::ActionType::INJURE) {
		zig ? position.x -= 0.010 * Globals::getResolution().x : position.x += 0.010 * Globals::getResolution().x;
		zig = !zig;
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
			resetFrameState(false);
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

void Character::determineAndSetGrabChar(Character* otherChar) {
	if (onSameVerticalPlane(otherChar->getCenter().y) &&
		spriteState == Globals::ActionType::MOVE &&
		abs((otherChar->getCenter().x - position.x) < 0.01f * Globals::getResolution().x) &&
		!otherChar->isInvincible()
		) {
		bool onRight = otherChar->getCenter().x > position.x ? true : false;
		if (onRight && (
			directionHeaded == DirectionHeaded::UR ||
			directionHeaded == DirectionHeaded::R ||
			directionHeaded == DirectionHeaded::DR
			)) {
			grabbedChar = otherChar;
			grabbing = true;
			grabbingFromBehind = otherChar->isFacingRight();
			otherChar->hold(true, this);
			setAttackState("grab", GRAB);
		}
		if (!onRight && (
			directionHeaded == DirectionHeaded::UL ||
			directionHeaded == DirectionHeaded::L ||
			directionHeaded == DirectionHeaded::DL
			)) {
			grabbedChar = otherChar;
			grabbing = true;
			grabbingFromBehind = !otherChar->isFacingRight();
			otherChar->hold(true, this);
			setAttackState("grab", GRAB);
		}
	}
}

void Character::resetGrab() {
	grabHits = 0;
	grabbing = false;
	grabbingFromBehind = false;
	if (grabbedChar != nullptr) {
		grabbedChar->hold(false);
		grabbedChar = nullptr;
	}
}

Character::DirectionHeaded Character::getDirectionOfCollision(Character* otherChar) {
	if (spriteState == Globals::ActionType::THROW) return directionHeaded;
	return otherChar->getCenter().x > position.x ? DirectionHeaded::R : DirectionHeaded::L;
}