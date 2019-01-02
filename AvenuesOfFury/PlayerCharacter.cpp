#include "pch.h"
#include "PlayerCharacter.h"
#include "SpriteHolder.h"

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
		setIdleState();
	}

	if (rightPressed && !leftPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		setMoveState();
	}

	if (leftPressed && !rightPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		setMoveState();
	}

	if (upPressed && !downPressed) {
		position.y -= speed * elapsedTime;
		setMoveState();
	}

	if (downPressed && !upPressed) {
		position.y += speed * elapsedTime;
		setMoveState();
	}

	if (primaryAttackPressed) {
		setAttackState(elapsedTime, ATTACK_1);
	}
	else if (secondaryAttackPressed) {
		setAttackState(elapsedTime, ATTACK_2);
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		setIdleState();
	}

	if (!primaryAttackPressed && !secondaryAttackPressed) {
		attackDisabled = false;
	}
}

void PlayerCharacter::disableInputs() {
	inputsDisabled = true;
	timeSinceLastAction = 0;
}

Vector2f PlayerCharacter::getPastPosition(int time) {
	return pastPositions[time];
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

void PlayerCharacter::hitEnemies(float elapsedTime, vector<EnemyCharacter*> enemies) {
	if (spriteState == Globals::ActionType::ATTACK) {
		for_each(enemies.begin(), enemies.end(), [&](EnemyCharacter* e) {
			if (!attackDisabled && hits(e)) {
				attackDisabled = true;
				e->registerHit(attackPower[currentActionType]);
			}
		});
	}
}

void PlayerCharacter::setMoveState() {
	currentAction = "move";
	currentActionType = MOVE_1;
	if (spriteState != Globals::ActionType::MOVE) {
		spriteState = Globals::ActionType::MOVE;
		resetFrameState();
	}
}

void PlayerCharacter::setIdleState() {
	currentAction = "idle";
	currentActionType = IDLE_1;
	if (spriteState != Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::IDLE;
		resetFrameState();
	}
}

void PlayerCharacter::setAttackState(float elapsedTime, int attackType) {
	timeSinceLastAction += elapsedTime * 1000;
	if (!attackDisabled) {
		timeSinceLastAction = 0;
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
}