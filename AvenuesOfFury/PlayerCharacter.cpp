#include "pch.h"
#include "PlayerCharacter.h"
#include "SpriteHolder.h"
#include <iostream>

PlayerCharacter::~PlayerCharacter() {
	delete[] pastDirectionsPressed;
}

void PlayerCharacter::update(float elapsedTime, vector<Character*> players, vector<Character*> enemies) {
	advanceHitRecords(elapsedTime);
	detectCollisions(players, enemies);
	updatePastPositions(elapsedTime);
	hitCharacters(elapsedTime);
	updateFrameState(elapsedTime, primaryAttackPressed);
	sprite.setPosition(position);
	render();

	if (disabled) {
		timeSinceLastAction += elapsedTime * 1000;
		if (timeSinceLastAction <= timeToBeDisabled) return;
		timeSinceLastAction = 0;
		disabled = false;
		attackDisabled = false;
		setIdleState(elapsedTime);
		insertAndShiftPastDirectionsPressed(DirectionHeaded::NONE);
		running = false;
	}

	if ((jumpPressed || jumping) && !jumpDisabled) {
		handleJump(elapsedTime);
		running = false;
		return;
	}

	if (!attackDisabled) {
		if (primaryAttackPressed) {
			setAttackState(HEAD_ATTACK);
			running = false;
			return;
		}
		else if (secondaryAttackPressed) {
			setAttackState(BODY_ATTACK);
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
		if (facingRight) flipHorizontally();
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

	DirectionHeaded current = stringToDirection(output);
	if ((current == DirectionHeaded::NONE && timeSinceLastDirectionPress < 100) ||
		(current == directionHeaded && timeSinceLastDirectionPress > 0) ||
		(directionHeaded == DirectionHeaded::NONE && current == DirectionHeaded::NONE)) {
		return;
	}

	insertAndShiftPastDirectionsPressed(current);
}

void PlayerCharacter::handleJump(float elapsedTime) {
	if (!Globals::isJumpingState(spriteState)) {
		spriteState = Globals::ActionType::JUMP_START;
		currentAction = "jump_start";
		currentActionType = JUMP_START;
		resetFrameState();
		jumping = true;
		running = false;
		prejumpY = position.y;
		position.y -= 0.1f;
		insertAndShiftPastDirectionsPressed(DirectionHeaded::NONE);
		speedY = baseSpeedY;
	}

	speedY += (.2 * gravity * (elapsedTime * 1000));
	if (prejumpY > position.y) position.y += speedY;
	if (rightPressed) position.x += speed * elapsedTime;
	if (leftPressed) position.x -= speed * elapsedTime;
}

void PlayerCharacter::hitCharacters(float elapsedTime) {
	if ((spriteState == Globals::ActionType::ATTACK || (spriteState == Globals::ActionType::JUMP_ATTACK) || (spriteState == Globals::ActionType::RUN_ATTACK)) &&
		playersTouching.size() + enemiesTouching.size() > 0) {
		vector<int> v = SpriteHolder::getDamageFramesForAction(spriteName, currentAction, currentActionType);
		if (find(v.begin(), v.end(), currentFrame) != v.end()) {
			for_each(enemiesTouching.begin(), enemiesTouching.end(), [&](Character* e) {
				AttackInfo info = generateAttackInfo(true, e);
				Vector2f target = e->getCenter();
				if (onSameVerticalPlane(target.y)) {
					e->registerHit(attackPower[currentActionType], spriteName, currentFrame, info);
					e->disable(info.timeToDisable);
					e->focusChar = this;
				}
			});
			for_each(playersTouching.begin(), playersTouching.end(), [&](Character* p) {
				AttackInfo info = generateAttackInfo(false, p);
				Vector2f target = p->getCenter();
				if (onSameVerticalPlane(target.y)) {
					p->registerHit(attackPower[currentActionType] * 0.05f, spriteName, currentFrame, info);
					p->disable(info.timeToDisable);
				}
			});
		}
	}
}

void PlayerCharacter::setMoveState(float elapsedTime) {
	timeSinceLastDirectionPress += elapsedTime * 1000;
	currentAction = "move";
	if ((pastDirectionsPressed[4] != DirectionHeaded::NONE) &&
		((pastDirectionsPressed[3] == pastDirectionsPressed[4]) || running)) {
		currentActionType = RUN;
		speed = baseSpeed * 2.0;
		running = true;
	}
	else {
		currentActionType = WALK;
		speed = baseSpeed;
		running = false;
	}
	if (spriteState != Globals::ActionType::MOVE) {
		spriteState = Globals::ActionType::MOVE;
		resetFrameState();
		timeSinceLastDirectionPress = 0;
	}
}

void PlayerCharacter::setIdleState(float elapsedTime) {
	currentAction = "idle";
	currentActionType = NORMAL_IDLE;
	if (spriteState != Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::IDLE;
		resetFrameState();
		timeSinceLastDirectionPress = 0;
	}
	timeSinceLastDirectionPress += elapsedTime * 1000;
}