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
		if (timeSinceLastAction <= STUN_LENGTH) {
			return;
		}
		timeSinceLastAction = 0;
		inputsDisabled = false;
		spriteState = Globals::ActionType::IDLE;
		currentAction = "idle";
		currentActionType = IDLE_1;
		resetFrameState();
	}

	if (rightPressed && !leftPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		currentAction = "move";
		currentActionType = MOVE_1;
		if (spriteState != Globals::ActionType::MOVE) {
			spriteState = Globals::ActionType::MOVE;
			resetFrameState();
		}
	}

	if (leftPressed && !rightPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		currentAction = "move";
		currentActionType = MOVE_1;
		if (spriteState != Globals::ActionType::MOVE) {
			spriteState = Globals::ActionType::MOVE;
			resetFrameState();
		}
	}

	if (upPressed && !downPressed) {
		position.y -= speed * elapsedTime;
		if (!rightPressed && !leftPressed) {
			currentAction = "move";
			currentActionType = MOVE_1;
			if (spriteState != Globals::ActionType::MOVE) {
				spriteState = Globals::ActionType::MOVE;
				resetFrameState();
			}
		}
	}

	if (downPressed && !upPressed) {
		position.y += speed * elapsedTime;
		if (!rightPressed && !leftPressed) {
			currentAction = "move";
			currentActionType = MOVE_1;
			if (spriteState != Globals::ActionType::MOVE) {
				spriteState = Globals::ActionType::MOVE;
				resetFrameState();
			}
		}
	}

	if (primaryAttackPressed) {
		handleAttack(elapsedTime);
		if (!attackDisabled) {
			timeSinceLastAction = 0;
			currentAction = "attack";
			if (currentActionType != ATTACK_1) {
				currentActionType = ATTACK_1;
				resetFrameState();
			} 
			if (spriteState != Globals::ActionType::ATTACK ) {
				spriteState = Globals::ActionType::ATTACK;
				resetFrameState();
			}
		}
	}
	else if (secondaryAttackPressed) {
		handleAttack(elapsedTime);
		if (!attackDisabled) {
			timeSinceLastAction = 0;
			currentAction = "attack";
			if (currentActionType != ATTACK_2) {
				currentActionType = ATTACK_2;
				resetFrameState();
			} 
			if (spriteState != Globals::ActionType::ATTACK) {
				spriteState = Globals::ActionType::ATTACK;
				resetFrameState();
			}
		}
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		currentAction = "idle";
		currentActionType = IDLE_1;
		if (spriteState != Globals::ActionType::IDLE) {
			spriteState = Globals::ActionType::IDLE;
			resetFrameState();
		}
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

void PlayerCharacter::handleAttack(float elapsedTime) {
	timeSinceLastAction += elapsedTime * 1000;
}