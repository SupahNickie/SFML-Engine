#include "pch.h"
#include "EnemyCharacter.h"
#include "SpriteHolder.h"

EnemyCharacter::EnemyCharacter(vector<Character*> players) {
	attackPower = vector<int>(1);
	focusChar = players[rand() % 2];
	spriteState = Globals::ActionType::IDLE;
	isActive = true;
	currentAction = "idle";
	currentActionType = IDLE_1;
	recalculateAggression();
	recalculateDecisionSpeed(false);
}

void EnemyCharacter::update(float elapsedTime, vector<Character*> players, vector<Character*> enemies) {
	if (health <= 0) {
		isActive = false;
		return;
	}
	detectCollisions(players, enemies);
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
	// going to be used for throwing
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
			for_each(enemiesTouching.begin(), enemiesTouching.end(), [&](Character* e) {
				Vector2f target = e->getCenter();
				if (find(v.begin(), v.end(), currentFrame) != v.end() &&
					(abs(target.y - position.y) < (.015625f * Globals::getResolution().x))) {
					e->registerHit(attackPower[currentActionType] * 0.05f);
					hitRegistered = true;
					e->disable();
				}
			});
			for_each(playersTouching.begin(), playersTouching.end(), [&](Character* p) {
				Vector2f target = p->getCenter();
				if (find(v.begin(), v.end(), currentFrame) != v.end() &&
					(abs(target.y - position.y) < (.015625f * Globals::getResolution().x))) {
					p->registerHit(attackPower[currentActionType]);
					hitRegistered = true;
					p->disable();
				}
			});
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

bool EnemyCharacter::handleDecidingState(float elapsedTime, vector<Character*> players) {
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

void EnemyCharacter::predictFocusCharLocation(float elapsedTime) {
	if (spriteState == Globals::ActionType::MOVE) {
		CharacterVelocity cv = focusChar->getVelocity(reactionSpeed);
		switch (cv.direction) {
		case Graphic::DirectionHeaded::U:
			target.x = cv.position.x;
			target.y = cv.position.y - focusChar->speed;
			break;
		case Graphic::DirectionHeaded::UR:
			target.x = cv.position.x + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Graphic::DirectionHeaded::R:
			target.x = cv.position.x + focusChar->speed;
			target.y = cv.position.y;
			break;
		case Graphic::DirectionHeaded::DR:
			target.x = cv.position.x + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Graphic::DirectionHeaded::D:
			target.x = cv.position.x;
			target.y = cv.position.y + focusChar->speed;
			break;
		case Graphic::DirectionHeaded::DL:
			target.x = cv.position.x - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Graphic::DirectionHeaded::L:
			target.x = cv.position.x - focusChar->speed;
			target.y = cv.position.y;
			break;
		case Graphic::DirectionHeaded::UL:
			target.x = cv.position.x - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case Graphic::DirectionHeaded::NONE:
			target = cv.position;
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

void EnemyCharacter::handleAI(float elapsedTime, vector<Character*> players) {
	if (spriteState != Globals::ActionType::ATTACK) timeSinceAttackEnded += elapsedTime * 1000;
	resetStateAfterFinishingAction();
	if (handleDecidingState(elapsedTime, players)) return;
	if (handleAttacking(elapsedTime)) return;
	if (checkDecidingState()) return;
	enterMovingState();
	handleMoving(elapsedTime);
}