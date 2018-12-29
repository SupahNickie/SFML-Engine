#include "pch.h"
#include "EnemyCharacter.h"
#include <iostream>

void EnemyCharacter::update(float elapsedTime, vector<PlayerCharacter*> players) {
	if (health <= 0) {
		isActive = false;
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
	if (timeSinceAttackBegan > STUN_LENGTH && !attackDisabled) {
		attackDisabled = true;
		focusChar->registerHit(attackPower[currentActionType], elapsedTime);
		focusChar->disableInputs();
	}
}

void EnemyCharacter::handleAI(float elapsedTime, vector<PlayerCharacter*> players) {
	if (spriteState != Globals::ActionType::ATTACK) timeSinceAttackEnded += elapsedTime * 1000;

	// Resetting states after finishing current action
	if (currentActionDone) {
		if (spriteState == Globals::ActionType::ATTACK) {
			timeSinceAttackEnded = 0;
			timeSinceDecision = 0;
		}
		spriteState = Globals::ActionType::IDLE;
		currentAction = "idle";
		currentActionType = IDLE_1;
		resetFrameState();
		attackDisabled = false;
		timeSinceAttackBegan = 0;
	}

	// Deciding which player to focus on, idle animation for a bit
	if (deciding) {
		if (timeSinceDecision == 0) {
			spriteState = Globals::ActionType::IDLE;
			currentAction = "idle";
			currentActionType = IDLE_1;
			resetFrameState();

			// get nearest player
			Vector2f player1coords = players[0]->getCenter();
			Vector2f player2coords = players[1]->getCenter();
			float player1closeness = abs(abs(player1coords.x - position.x) - abs(player1coords.y - position.y));
			float player2closeness = abs(abs(player2coords.x - position.x) - abs(player2coords.y - position.y));
			if (player1closeness < player2closeness) {
				focusChar = players[0];
			}
			else {
				focusChar = players[1];
			}
			++timeSinceDecision;
		}
		if (timeSinceDecision > 500) {
			timeSinceDecision = 0;
			deciding = false;
		}
		timeSinceDecision += elapsedTime * 1000;
		return;
	}
	timeSinceDecision += elapsedTime * 1000;

	// Attacking
	if (!attackDisabled &&
		(timeSinceAttackEnded > aggression) &&
		spriteState != Globals::ActionType::INJURE &&
		hits(focusChar)
		) {
		attack(elapsedTime);
		return;
	}

	// Check decision timer and act accordingly
	if (timeSinceDecision > 3000) {
		deciding = true;
		timeSinceDecision = 0;
		return;
	}

	// Idle state, get the enemy moving
	if (spriteState == Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::MOVE;
		currentAction = "move";
		currentActionType = MOVE_1;
		resetFrameState();
	}

	// Move towards focused player if they are not touching or within vertical threshold
	if (spriteState == Globals::ActionType::MOVE) {
		Vector2f focusCharCoords = focusChar->getCenter();
		if ((abs(focusCharCoords.y - position.y) > (.015625f * Globals::getResolution().x)) ||
			!hits(focusChar)) {
			if (focusCharCoords.x > position.x) {
				position.x += elapsedTime * speed;
			}
			else if (focusCharCoords.x < position.x) {
				position.x -= elapsedTime * speed;
			}
			if (focusCharCoords.y > position.y) {
				position.y += elapsedTime * speed;
			}
			else if (focusCharCoords.y < position.y) {
					position.y -= elapsedTime * speed;
			}
		}
	}
}