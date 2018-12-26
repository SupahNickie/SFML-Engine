#include "pch.h"
#include "EnemyCharacter.h"
#include <iostream>

void EnemyCharacter::update(float elapsedTime, PlayerCharacter** players) {
	if (health <= 0) {
		isActive = false;
		deleteSprite();
		return;
	}
	if (isActive) {
		updateFrameState(elapsedTime);
		turnToFaceFocusChar();
		handleAI(elapsedTime, players);
		sprite.setPosition(position);
		render();
	}
}

void EnemyCharacter::turnToFaceFocusChar() {
	bool playerToRightOfSelf = focusChar->getCenter().x > this->getCenter().x;
	if (playerToRightOfSelf && facingLeft) {
		flipHorizontally();
	}
	else if (!playerToRightOfSelf && facingRight) {
		flipHorizontally();
	}
}

void EnemyCharacter::attack(float elapsedTime) {
	spriteState = Globals::ActionType::ATTACK;
	if (timeSinceAttackBegan != 0) timeSinceAttackBegan += elapsedTime * 1000;
	if (timeSinceAttackBegan > 1000) {
		timeSinceAttackBegan = 0;
		currentActionDone = true;
		attackDisabled = false;
		return;
	}
	if (!attackDisabled && hits(focusChar)) {
		if (timeSinceAttackBegan == 0) {
			currentAction = "attack";
			currentActionType = ATTACK_1;
			resetFrameState();
			++timeSinceAttackBegan;
			attackDisabled = true;
		}
		focusChar->registerHit(1, elapsedTime);
		focusChar->disableInputs();
	}
}

void EnemyCharacter::handleAI(float elapsedTime, PlayerCharacter** players) {
	timeSinceDecision += elapsedTime * 1000;
	if (spriteState != Globals::ActionType::ATTACK) timeSinceAttackEnded += elapsedTime * 1000;
	if (currentActionDone) {
		spriteState = Globals::ActionType::IDLE;
		currentAction = "idle";
		currentActionType = IDLE_1;
		resetFrameState();
	}
}