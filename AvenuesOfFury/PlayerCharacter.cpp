#include "pch.h"
#include "PlayerCharacter.h"
#include "SpriteHolder.h"

void PlayerCharacter::update(float elapsedTime, EnemyCharacter** enemies, int numEnemies) {
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
		handleAttack();
		if (!attackDisabled) {
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
		handleAttack();
		if (!attackDisabled) {
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
	else if (spriteState == Globals::ActionType::ATTACK && !primaryAttackPressed && !secondaryAttackPressed) {
		currentAction = "idle";
		currentActionType = IDLE_1;
		if (spriteState != Globals::ActionType::IDLE) {
			spriteState = Globals::ActionType::IDLE;
			resetFrameState();
		}
		attackDisabled = false;
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		currentAction = "idle";
		currentActionType = IDLE_1;
		if (spriteState != Globals::ActionType::IDLE) {
			spriteState = Globals::ActionType::IDLE;
			resetFrameState();
		}
	}

	updateFrameState(elapsedTime);
	sprite.setPosition(position);
	render();

	for (int i = 0; i < numEnemies; ++i) {
		if (enemies[i]->isActive) {
			if (hits(enemies [i])) {
				enemies[i]->registerHit(1, elapsedTime);
			}
		}
	}
}

void PlayerCharacter::handleAttack() {

}