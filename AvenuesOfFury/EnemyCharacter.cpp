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
	if (timeSinceAttackBegan == 0) {
		spriteState = Globals::ActionType::ATTACK;
		currentAction = "attack";
		currentActionType = ATTACK_1;
		resetFrameState();
		++timeSinceAttackBegan;
	}
	timeSinceAttackBegan += elapsedTime * 1000;
	if (timeSinceAttackBegan > (MS_PER_FRAME * 2) && !attackDisabled) {
		attackDisabled = true;
		focusChar->registerHit(attackPower[currentActionType], elapsedTime);
		focusChar->disableInputs();
	}
}

void EnemyCharacter::handleAI(float elapsedTime, PlayerCharacter** players) {
	timeSinceDecision += elapsedTime * 1000;
	if (spriteState != Globals::ActionType::ATTACK) timeSinceAttackEnded += elapsedTime * 1000;
	if (currentActionDone) {
		if (spriteState == Globals::ActionType::ATTACK) timeSinceAttackEnded = 0;
		spriteState = Globals::ActionType::IDLE;
		currentAction = "idle";
		currentActionType = IDLE_1;
		resetFrameState();
		attackDisabled = false;
		timeSinceAttackBegan = 0;
	}
	if (!attackDisabled &&
		(timeSinceAttackEnded > aggression) &&
		spriteState != Globals::ActionType::INJURE &&
		hits(focusChar)
		) {
		attack(elapsedTime);
	}
}