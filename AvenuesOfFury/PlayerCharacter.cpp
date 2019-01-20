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
		if (timeSinceLastAction <= timeToBeDisabled) return;
		timeSinceLastAction = 0;
		disabled = false;
		attackDisabled = false;
		setIdleState(elapsedTime);
		insertAndShiftPastDirectionsPressed(DirectionHeaded::NONE);
		running = false;
	}

	if (runAttacking) return;

	if ((jumpPressed || jumping) && !jumpDisabled) {
		setJumpState(elapsedTime, leftPressed, rightPressed);
		running = false;
		return;
	}

	if (!attackDisabled) {
		if (primaryAttackPressed) {
			running ? setAttackState("run_attack", RUN_ATTACK) : setAttackState("attack", HEAD_ATTACK);
			return;
		}
		else if (secondaryAttackPressed) {
			setAttackState("attack", BODY_ATTACK);
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

	updateHandled = false;
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

	if (!updateHandled) setIdleState(elapsedTime);
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
	updateHandled = true;
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
	updateHandled = true;
	currentAction = "idle";
	currentActionType = NORMAL_IDLE;
	if (spriteState != Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::IDLE;
		resetFrameState();
		timeSinceLastDirectionPress = 0;
	}
	timeSinceLastDirectionPress += elapsedTime * 1000;
}