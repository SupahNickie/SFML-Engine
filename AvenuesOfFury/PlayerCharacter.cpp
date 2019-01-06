#include "pch.h"
#include "PlayerCharacter.h"
#include "SpriteHolder.h"
#include <iostream>

PlayerCharacter::PlayerCharacter() {
	// Init array of direction presses remembered
	pastDirectionsPressed = new Graphic::DirectionHeaded[5];
	pastDirectionsPressed[0] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[1] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[2] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[3] = Graphic::DirectionHeaded::NONE;
	pastDirectionsPressed[4] = Graphic::DirectionHeaded::NONE;
}

PlayerCharacter::~PlayerCharacter() {
	delete[] pastDirectionsPressed;
}

void PlayerCharacter::update(float elapsedTime, vector<Character*> players, vector<Character*> enemies) {
	detectCollisions(players, enemies);
	updatePastPositions(elapsedTime);
	hitCharacters(elapsedTime);
	updateFrameState(elapsedTime);
	sprite.setPosition(position);
	render();

	if (disabled) {
		timeSinceLastAction += elapsedTime * 1000;
		if (timeSinceLastAction <= STUN_LENGTH) return;
		timeSinceLastAction = 0;
		disabled = false;
		attackDisabled = false;
		setIdleState(elapsedTime);
		memmove(&pastDirectionsPressed[0], &pastDirectionsPressed[1], (size_t)4 * sizeof(pastDirectionsPressed[0]));
		pastDirectionsPressed[4] = Graphic::DirectionHeaded::NONE;
		running = false;
	}

	if (!attackDisabled) {
		if (primaryAttackPressed) {
			setAttackState(elapsedTime, ATTACK_1);
			running = false;
			return;
		}
		else if (secondaryAttackPressed) {
			setAttackState(elapsedTime, ATTACK_2);
			running = false;
			return;
		}
	}
	else if (primaryAttackPressed || secondaryAttackPressed) {
		if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
			setIdleState(elapsedTime);
			running = false;
		}
		else {
			setMoveState(elapsedTime);
		}
	}
	else {
		attackDisabled = false;
	}

	if (rightPressed && !leftPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		setMoveState(elapsedTime);
	}

	if (leftPressed && !rightPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		setMoveState(elapsedTime);
	}

	if (upPressed && !downPressed) {
		position.y -= speed * elapsedTime;
		setMoveState(elapsedTime);
	}

	if (downPressed && !upPressed) {
		position.y += speed * elapsedTime;
		setMoveState(elapsedTime);
	}

	if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		setIdleState(elapsedTime);
		running = false;
	}
	setDirectionHeaded();
}

void PlayerCharacter::setDirectionHeaded() {
	string output = "";
	if (upPressed && !downPressed) output += "U";
	if (downPressed && !upPressed) output += "D";
	if (leftPressed && !rightPressed) output += "L";
	if (rightPressed && !leftPressed) output += "R";

	Graphic::DirectionHeaded current = stringToDirection(output);
	if ((current == Graphic::DirectionHeaded::NONE && timeSinceLastDirectionPress < 100) ||
		(current == directionHeaded && timeSinceLastDirectionPress > 0) ||
		(directionHeaded == Graphic::DirectionHeaded::NONE && current == Graphic::DirectionHeaded::NONE)) {
		return;
	}

	directionHeaded = current;
	memmove(&pastDirectionsPressed[0], &pastDirectionsPressed[1], (size_t)4 * sizeof(pastDirectionsPressed[0]));
	pastDirectionsPressed[4] = directionHeaded;
}

void PlayerCharacter::hitCharacters(float elapsedTime) {
	if (spriteState == Globals::ActionType::ATTACK &&
		playersTouching.size() + enemiesTouching.size() > 0 &&
		!hitRegistered) {
		vector<int> v = SpriteHolder::getDamageFramesForAction(spriteName, currentAction, currentActionType);
		for_each(enemiesTouching.begin(), enemiesTouching.end(), [&](Character* e) {
			if (find(v.begin(), v.end(), currentFrame) != v.end()) {
				e->registerHit(attackPower[currentActionType]);
				hitRegistered = true;
			}
		});
		for_each(playersTouching.begin(), playersTouching.end(), [&](Character* p) {
			if (find(v.begin(), v.end(), currentFrame) != v.end()) {
				p->registerHit(attackPower[currentActionType]);
				hitRegistered = true;
			}
		});
	}
}

void PlayerCharacter::setMoveState(float elapsedTime) {
	timeSinceLastDirectionPress += elapsedTime * 1000;
	currentAction = "move";
	if ((pastDirectionsPressed[4] != Graphic::DirectionHeaded::NONE) &&
		((pastDirectionsPressed[3] == pastDirectionsPressed[4]) || running)) {
		currentActionType = MOVE_2;
		speed = baseSpeed * 2.0;
		running = true;
	}
	else {
		currentActionType = MOVE_1;
		speed = baseSpeed;
		running = false;
	}
	if (spriteState != Globals::ActionType::MOVE) {
		spriteState = Globals::ActionType::MOVE;
		resetFrameState();
		timeSinceLastDirectionPress = 0;
	}
	hitRegistered = false;
}

void PlayerCharacter::setIdleState(float elapsedTime) {
	currentAction = "idle";
	currentActionType = IDLE_1;
	if (spriteState != Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::IDLE;
		resetFrameState();
		timeSinceLastDirectionPress = 0;
	}
	timeSinceLastDirectionPress += elapsedTime * 1000;
	hitRegistered = false;
}

void PlayerCharacter::setAttackState(float elapsedTime, int attackType) {
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