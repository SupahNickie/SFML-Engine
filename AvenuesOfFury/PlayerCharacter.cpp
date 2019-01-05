#include "pch.h"
#include "PlayerCharacter.h"
#include "SpriteHolder.h"
#include <iostream>

PlayerCharacter::PlayerCharacter() {
	pastDirectionsPressed = new Character::DirectionHeaded[5];
	pastDirectionsPressed[0] = Character::DirectionHeaded::NONE;
	pastDirectionsPressed[1] = Character::DirectionHeaded::NONE;
	pastDirectionsPressed[2] = Character::DirectionHeaded::NONE;
	pastDirectionsPressed[3] = Character::DirectionHeaded::NONE;
	pastDirectionsPressed[4] = Character::DirectionHeaded::NONE;
}

PlayerCharacter::~PlayerCharacter() {
	delete[] pastDirectionsPressed;
}

void PlayerCharacter::update(float elapsedTime, vector<EnemyCharacter*> enemies) {
	updatePastPositions(elapsedTime);
	hitEnemies(elapsedTime, enemies);
	updateFrameState(elapsedTime);
	sprite.setPosition(position);
	render();

	if (inputsDisabled) {
		timeSinceLastAction += elapsedTime * 1000;
		if (timeSinceLastAction <= STUN_LENGTH) return;
		timeSinceLastAction = 0;
		inputsDisabled = false;
		attackDisabled = false;
		setIdleState(elapsedTime);
	}

	if (!attackDisabled) {
		if (primaryAttackPressed) {
			setAttackState(elapsedTime, ATTACK_1);
			return;
		}
		else if (secondaryAttackPressed) {
			setAttackState(elapsedTime, ATTACK_2);
			return;
		}
	}
	else if (primaryAttackPressed || secondaryAttackPressed) {
		if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
			setIdleState(elapsedTime);
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
	}
	setDirectionHeaded();
}

void PlayerCharacter::disableInputs() {
	inputsDisabled = true;
	timeSinceLastAction = 0;
}

PlayerPastPosition PlayerCharacter::getPastPosition(int time) {
	PlayerPastPosition output;
	output.position = pastPositions[time];
	output.direction = directionHeaded;
	return output;
}

void PlayerCharacter::updatePastPositions(float elapsedTime) {
	if (timeSincePastPositionsUpdate > 50) {
		int positionToUpdate = 500;
		int positionToGrab = 450;
		while (positionToUpdate > 0) {
			pastPositions[positionToUpdate] = pastPositions[positionToGrab];
			positionToUpdate = positionToGrab;
			positionToGrab -= 50;
		}
		pastPositions[0] = position;
		timeSincePastPositionsUpdate = 0;
	}
	timeSincePastPositionsUpdate += elapsedTime * 1000;
}

void PlayerCharacter::setDirectionHeaded() {
	string output = "";
	if (upPressed && !downPressed) output += "U";
	if (downPressed && !upPressed) output += "D";
	if (leftPressed && !rightPressed) output += "L";
	if (rightPressed && !leftPressed) output += "R";

	Character::DirectionHeaded current = stringToDirection(output);
	if ((current == Character::DirectionHeaded::NONE && timeSinceLastDirectionPress < 200) ||
		(current == directionHeaded && timeSinceLastDirectionPress > 0) ||
		(directionHeaded == Character::DirectionHeaded::NONE && current == Character::DirectionHeaded::NONE)) {
		return;
	}

	directionHeaded = current;
	memmove(&pastDirectionsPressed[0], &pastDirectionsPressed[1], (size_t)4 * sizeof(pastDirectionsPressed[0]));
	pastDirectionsPressed[4] = directionHeaded;
}

void PlayerCharacter::hitEnemies(float elapsedTime, vector<EnemyCharacter*> enemies) {
	if (spriteState == Globals::ActionType::ATTACK) {
		for_each(enemies.begin(), enemies.end(), [&](EnemyCharacter* e) {
			if (!hitRegistered && hits(e)) {
				vector<int> v = SpriteHolder::getDamageFramesForAction(spriteName, currentAction, currentActionType);
				if (find(v.begin(), v.end(), currentFrame) != v.end()) {
					e->registerHit(attackPower[currentActionType]);
					hitRegistered = true;
				}
			}
		});
	}
}

void PlayerCharacter::setMoveState(float elapsedTime) {
	timeSinceLastDirectionPress += elapsedTime * 1000;
	currentAction = "move";
	currentActionType = MOVE_1;
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