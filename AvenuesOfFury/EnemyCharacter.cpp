#include "pch.h"
#include "EnemyCharacter.h"
#include "SpriteHolder.h"

EnemyCharacter::EnemyCharacter(vector<PlayerCharacter*> players) {
	attackPower = vector<int>(1);
	focusChar = players[rand() % 2];
	spriteState = Globals::ActionType::IDLE;
	isActive = true;
	currentAction = "idle";
	currentActionType = IDLE_1;
	recalculateAggression();
	recalculateDecisionSpeed(false);
}

void EnemyCharacter::update(float elapsedTime, vector<PlayerCharacter*> players) {
	if (health <= 0) {
		isActive = false;
		return;
	}
	updateFrameState(elapsedTime);
	turnToFaceFocusChar();
	handleAI(elapsedTime, players);
	sprite.setPosition(position);
	render();
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

void EnemyCharacter::setDirectionHeaded() {

}

void EnemyCharacter::attack(float elapsedTime) {
	if (timeSinceAttackBegan == 0) {
		spriteState = Globals::ActionType::ATTACK;
		currentAction = "attack";
		currentActionType = ATTACK_1;
		resetFrameState();
		++timeSinceAttackBegan;
		recalculateAggression();
	}
	timeSinceAttackBegan += elapsedTime * 1000;
	if (!hitRegistered) {
		vector<int> v = SpriteHolder::getDamageFramesForAction(spriteName, currentAction, currentActionType);
		if (find(v.begin(), v.end(), currentFrame) != v.end()) {
			hitRegistered = true;
			focusChar->registerHit(attackPower[currentActionType]);
			focusChar->disableInputs();
		}
	}
}

void EnemyCharacter::recalculateAggression() {
	varianceAggression = rand() % maxAggression;
	aggression = baseAggression + varianceAggression;
}

void EnemyCharacter::recalculateDecisionSpeed(bool decisionState) {
	deciding = decisionState;
	timeSinceDecision = 0;
	varianceDecision = rand() % maxDecisionSpeed;
	decisionSpeed = baseDecisionSpeed + varianceDecision;
}

void EnemyCharacter::resetStateAfterFinishingAction() {
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
}

bool EnemyCharacter::handleDecidingState(float elapsedTime, vector<PlayerCharacter*> players) {
	if (decisionSpeed != 0 && deciding) {
		if (timeSinceDecision == 0) {
			spriteState = Globals::ActionType::IDLE;
			currentAction = "idle";
			currentActionType = IDLE_1;
			resetFrameState();

			// get nearest player
			Vector2f player1coords = players[0]->getVelocity(reactionSpeed).position;
			Vector2f player2coords = players[1]->getVelocity(reactionSpeed).position;
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
		if (timeSinceDecision > decisionSpeed) {
			recalculateDecisionSpeed(false);
		}
		timeSinceDecision += elapsedTime * 1000;
		return true;
	}
	else {
		timeSinceDecision += elapsedTime * 1000;
		return false;
	}
}

bool EnemyCharacter::basicAttack(float elapsedTime) {
	if (!attackDisabled &&
		(timeSinceAttackEnded > aggression) &&
		spriteState != Globals::ActionType::INJURE &&
		hits(focusChar)
		) {
		attack(elapsedTime);
		return true;
	}
	return false;
}

bool EnemyCharacter::checkDecidingState() {
	if (decisionSpeed != 0 && timeSinceDecision > decisionSpeed) {
		recalculateDecisionSpeed(true);
		return true;
	}
	return false;
}

void EnemyCharacter::enterMovingState() {
	if (spriteState == Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::MOVE;
		currentAction = "move";
		currentActionType = MOVE_1;
		resetFrameState();
	}
}

void EnemyCharacter::moveTowardsFocusChar(float elapsedTime) {
	if (spriteState == Globals::ActionType::MOVE) {
		target = focusChar->getVelocity(reactionSpeed).position;
		if ((abs(target.y - position.y) > (.015625f * Globals::getResolution().x)) ||
			!hits(focusChar)) {
			moveTowardsTarget(elapsedTime);
		}
	}
}

void EnemyCharacter::predictPlayerLocation(float elapsedTime) {
	if (spriteState == Globals::ActionType::MOVE) {
		PlayerVelocity pv = focusChar->getVelocity(reactionSpeed);
		switch (pv.direction) {
		case Character::DirectionHeaded::U:
			target.x = pv.position.x;
			target.y = pv.position.y - focusChar->speed;
			break;
		case Character::DirectionHeaded::UR:
			target.x = pv.position.x + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = pv.position.y - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Character::DirectionHeaded::R:
			target.x = pv.position.x + focusChar->speed;
			target.y = pv.position.y;
			break;
		case Character::DirectionHeaded::DR:
			target.x = pv.position.x + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = pv.position.y + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Character::DirectionHeaded::D:
			target.x = pv.position.x;
			target.y = pv.position.y + focusChar->speed;
			break;
		case Character::DirectionHeaded::DL:
			target.x = pv.position.x - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = pv.position.y + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Character::DirectionHeaded::L:
			target.x = pv.position.x - focusChar->speed;
			target.y = pv.position.y;
			break;
		case Character::DirectionHeaded::UL:
			target.x = pv.position.x - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = pv.position.y - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Character::DirectionHeaded::NONE:
			target = pv.position;
			break;
		}
	}
	if ((abs(target.y - position.y) > (.015625f * Globals::getResolution().x)) ||
		!hits(focusChar)) {
		moveTowardsTarget(elapsedTime);
	}
}

void EnemyCharacter::moveTowardsTarget(float elapsedTime) {
	if (target.x > position.x) {
		position.x += elapsedTime * speed;
	}
	else if (target.x < position.x) {
		position.x -= elapsedTime * speed;
	}
	if (target.y > position.y) {
		position.y += elapsedTime * speed;
	}
	else if (target.y < position.y) {
		position.y -= elapsedTime * speed;
	}
}

void EnemyCharacter::handleAI(float elapsedTime, vector<PlayerCharacter*> players) {
	if (spriteState != Globals::ActionType::ATTACK) timeSinceAttackEnded += elapsedTime * 1000;
	resetStateAfterFinishingAction();
	if (handleDecidingState(elapsedTime, players)) return;
	if (handleAttacking(elapsedTime)) return;
	if (checkDecidingState()) return;
	enterMovingState();
	handleMoving(elapsedTime);
}