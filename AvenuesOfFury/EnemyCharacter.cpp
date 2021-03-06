#include "pch.h"
#include "EnemyCharacter.h"
#include "SpriteHolder.h"
#include <iostream>

EnemyCharacter::EnemyCharacter(vector<Character*> players) {
	attackPower = vector<int>(1);
	focusChar = players[rand() % 2];
	spriteState = Globals::ActionType::IDLE;
	currentAction = "idle";
	currentActionType = NORMAL_IDLE;
	recalculateAggression();
	recalculateDecisionSpeed(false);
}

void EnemyCharacter::update(float elapsedTime, vector<Character*> players, vector<Character*> enemies) {
	if (health <= 0) {
		isActive = false;
		return;
	}
	advanceHitRecords(elapsedTime);
	detectCollisions(players, enemies);
	updateFrameState(elapsedTime, jumpAttacking ? "jumpAttacking" : "" );
	handleAI(elapsedTime, players);
	render();
}

void EnemyCharacter::calculateAttack(float elapsedTime) {
	if (timeSinceAttackBegan == 0) recalculateAggression();

	timeSinceAttackBegan += elapsedTime * 1000;
	vector<int> v = SpriteHolder::getDamageFramesForAction(getSpriteName(), currentAction, currentActionType);
	if (find(v.begin(), v.end(), currentFrame) != v.end()) {
		for_each(enemiesTouching.begin(), enemiesTouching.end(), [&](Character* e) {
			AttackInfo info = generateAttackInfo(true, e);
			Vector2f target = e->getPosition();
			if (onSameVerticalPlane(target.y)) {
				e->registerHit(attackPower[currentActionType] * 0.05f, getSpriteName(), currentFrame, info);
				e->disable(info.timeToDisable);
			}
		});
		for_each(playersTouching.begin(), playersTouching.end(), [&](Character* p) {
			AttackInfo info = generateAttackInfo(false, p);
			Vector2f target = p->getPosition();
			if (onSameVerticalPlane(target.y)) {
				p->registerHit(attackPower[currentActionType], getSpriteName(), currentFrame, info);
				p->disable(info.timeToDisable);
			}
		});
	}
}

bool EnemyCharacter::checkDecidingState() {
	if (decisionSpeed != 0 && timeSinceDecision > decisionSpeed) {
		recalculateDecisionSpeed(true);
		return true;
	}
	return false;
}

bool EnemyCharacter::handleDecidingState(float elapsedTime, vector<Character*> players) {
	if (decisionSpeed != 0 && deciding) {
		if (timeSinceDecision == 0) {
			spriteState = Globals::ActionType::IDLE;
			currentAction = "idle";
			currentActionType = NORMAL_IDLE;
			resetFrameState();

			// get nearest player
			Vector2f player1coords = players[0]->getVelocity(reactionSpeed).position;
			Vector2f player2coords = players[1]->getVelocity(reactionSpeed).position;
			float player1closeness = abs(abs(player1coords.x - getPosition().x) - abs(player1coords.y - getPosition().y));
			float player2closeness = abs(abs(player2coords.x - getPosition().x) - abs(player2coords.y - getPosition().y));
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

void EnemyCharacter::moveTowardsFocusChar(float elapsedTime) {
	if (spriteState == Globals::ActionType::MOVE) {
		target = focusChar->getVelocity(reactionSpeed).position;
		if (!onSameVerticalPlane(target.y)) {
			moveTowardsTarget(elapsedTime);
		}
		else if (onSameVerticalPlane(target.y) && !hits(focusChar)) {
			moveTowardsTarget(elapsedTime);
		}
	}
}

void EnemyCharacter::predictFocusCharLocation(float elapsedTime) {
	if (spriteState == Globals::ActionType::MOVE) {
		CharacterVelocity cv = focusChar->getVelocity(reactionSpeed);
		switch (cv.direction) {
		case DirectionHeaded::U:
			target.x = cv.position.x;
			target.y = cv.position.y - focusChar->speed;
			break;
		case DirectionHeaded::UR:
			target.x = cv.position.x + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case DirectionHeaded::R:
			target.x = cv.position.x + focusChar->speed;
			target.y = cv.position.y;
			break;
		case DirectionHeaded::DR:
			target.x = cv.position.x + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case DirectionHeaded::D:
			target.x = cv.position.x;
			target.y = cv.position.y + focusChar->speed;
			break;
		case DirectionHeaded::DL:
			target.x = cv.position.x - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y + (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case DirectionHeaded::L:
			target.x = cv.position.x - focusChar->speed;
			target.y = cv.position.y;
			break;
		case DirectionHeaded::UL:
			target.x = cv.position.x - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			target.y = cv.position.y - (sqrt(pow(focusChar->speed, 2.0) / 2.0));
			break;
		case DirectionHeaded::NONE:
			target = cv.position;
			break;
		}
	}
	if (!onSameVerticalPlane(target.y)) {
		moveTowardsTarget(elapsedTime);
	}
	else if (onSameVerticalPlane(target.y) && !hits(focusChar)) {
		moveTowardsTarget(elapsedTime);
	}
}

void EnemyCharacter::handleAI(float elapsedTime, vector<Character*> players) {
	if (spriteState != Globals::ActionType::ATTACK) timeSinceAttackEnded += elapsedTime * 1000;
	if (spriteState == Globals::ActionType::FALL) handleFallDamage(elapsedTime);
	if (handleDisabledState(elapsedTime)) return;
	turnToFaceFocusChar();
	resetStateAfterFinishingAction();
	if (handleDecidingState(elapsedTime, players)) return;
	if (handleAttacking(elapsedTime)) return;
	if (checkDecidingState()) return;
	setMoveState();
	handleMoving(elapsedTime);
}

bool EnemyCharacter::attack(float elapsedTime, int actionType) {
	if (timeSinceAttackEnded > aggression) {
		if (!attackDisabled &&
			spriteState != Globals::ActionType::INJURE &&
			hits(focusChar) &&
			onSameVerticalPlane(focusChar->getPosition().y)
			) {
			setAttackState("attack", actionType);
			calculateAttack(elapsedTime);
			return true;
		}
		return false;
	}
	else if (spriteState == Globals::ActionType::INJURE) {
		return true;
	}
	else {
		setIdleState(elapsedTime);
		return true;
	}
}

bool EnemyCharacter::jumpAttack(float elapsedTime) {
	if (timeSinceAttackEnded > aggression) {
		if (jumping) {
			handleJump(elapsedTime);
			return true;
		}
		if (!attackDisabled &&
			spriteState != Globals::ActionType::INJURE &&
			onSameVerticalPlane(focusChar->getPosition().y)
			) {
			handleJump(elapsedTime);
			return true;
		}
		return false;
	}
	else if (spriteState == Globals::ActionType::INJURE) {
		return true;
	}
	else {
		setIdleState(elapsedTime);
		return false;
	}
}

bool EnemyCharacter::grabAttack(float elapsedTime) {
	if (timeSinceAttackEnded > aggression) {
		if (grabbing) {
			handleGrab(elapsedTime);
			return true;
		}
		if (!attackDisabled &&
			spriteState != Globals::ActionType::INJURE &&
			hits(focusChar) &&
			onSameVerticalPlane(focusChar->getPosition().y)
			) {
			handleGrab(elapsedTime);
			return true;
		}
		return false;
	}
	else if (spriteState == Globals::ActionType::INJURE) {
		return true;
	}
	else {
		setIdleState(elapsedTime);
		return false;
	}
}

void EnemyCharacter::setIdleState(float elapsedTime) {
	currentAction = "idle";
	currentActionType = NORMAL_IDLE;
	if (spriteState != Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::IDLE;
		resetFrameState();
	}
}

void EnemyCharacter::setMoveState() {
	if (spriteState == Globals::ActionType::IDLE) {
		spriteState = Globals::ActionType::MOVE;
		currentAction = "move";
		currentActionType = WALK;
		resetFrameState();
	}
}

void EnemyCharacter::turnToFaceFocusChar() {
	if (Globals::isJumpingState(spriteState)) return;
	bool playerToRightOfSelf = focusChar->getPosition().x > this->getPosition().x;
	if (playerToRightOfSelf && !facingRight) {
		flipHorizontally();
	}
	else if (!playerToRightOfSelf && facingRight) {
		flipHorizontally();
	}
}

void EnemyCharacter::setDirectionHeaded() {
	int direction = rand() % 8;
	switch (direction) {
	case(0):
		directionHeaded = DirectionHeaded::D;
		break;
	case(1):
		directionHeaded = DirectionHeaded::U;
		break;
	case(2):
		directionHeaded = DirectionHeaded::L;
		break;
	case(3):
		directionHeaded = DirectionHeaded::R;
		break;
	case(4):
		directionHeaded = DirectionHeaded::UL;
		break;
	case(5):
		directionHeaded = DirectionHeaded::DR;
		break;
	case(6):
		directionHeaded = DirectionHeaded::UR;
		break;
	case(7):
		directionHeaded = DirectionHeaded::DL;
		break;
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
		if (spriteState == Globals::ActionType::ATTACK ||
			spriteState == Globals::ActionType::JUMP_ATTACK ||
			spriteState == Globals::ActionType::GRAB_ATTACK_HEAD ||
			spriteState == Globals::ActionType::GRAB_ATTACK_BODY ||
			spriteState == Globals::ActionType::THROW
			) {
			timeSinceAttackEnded = 0;
			timeSinceDecision = 0;
		}
		if (spriteState == Globals::ActionType::THROW) {
			grabbing = false;
		}
		spriteState = Globals::ActionType::IDLE;
		currentAction = "idle";
		currentActionType = NORMAL_IDLE;
		resetFrameState();
		jumping = false;
		attackDisabled = false;
		timeSinceAttackBegan = 0;
	}
}

void EnemyCharacter::handleFallDamage(float elapsedTime) {
	if (attackedBy != nullptr) {
		auto it = remove_if(playersTouching.begin(), playersTouching.end(), [&](Character* p) {
			return p->uniqueID == attackedBy->uniqueID;
		});
		playersTouching.erase(it, playersTouching.end());
		calculateAttack(elapsedTime);
	}
}

bool EnemyCharacter::handleDisabledState(float elapsedTime) {
	if (held) return true;
	if (disabled) {
		timeSinceLastAction += elapsedTime * 1000;
		if (timeSinceLastAction <= timeToBeDisabled) return true;
		timeSinceLastAction = 0;
		timeToBeDisabled = 0;
		disabled = false;
		attackDisabled = false;
		jumpAttacking = false;
		jumpDisabled = false;
		if (!grabbing) setIdleState(elapsedTime);
		return false;
	}
	return false;
}

void EnemyCharacter::handleJump(float elapsedTime) {
	setJumpState(elapsedTime, focusChar->getPosition().x < getPosition().x, focusChar->getPosition().x > getPosition().x);
	if (hits(focusChar)) {
		jumpAttacking = true;
		setAttackState("jump_attack", JUMP_ATTACK);
		calculateAttack(elapsedTime);
	}
}

void EnemyCharacter::handleGrab(float elapsedTime) {
	if (!hits(focusChar) || grabHits > 2 || currentAction == "throw") {
		timeSinceAttackEnded = 0;
		resetGrab();
		return;
	}
	if (!grabbing) {
		grabbedChar = focusChar;
		grabbing = true;
	}
	int choice = rand() % 3;
	switch (choice) {
		case(0):
			setAttackState("grab_attack_head", GRAB_ATTACK_HEAD);
			disable(MS_PER_FRAME * SpriteHolder::getMaxFramesForAction(getSpriteName(), "grab_attack_head", GRAB_ATTACK_HEAD));
			grabbedChar->hold(false);
			break;
		case(1):
			setAttackState("grab_attack_body", GRAB_ATTACK_BODY);
			disable(MS_PER_FRAME * SpriteHolder::getMaxFramesForAction(getSpriteName(), "grab_attack_body", GRAB_ATTACK_BODY));
			grabbedChar->hold(false);
			break;
		case(2):
			setAttackState("throw", THROW);
			disable(MS_PER_FRAME * SpriteHolder::getMaxFramesForAction(getSpriteName(), "throw", THROW));
			setDirectionHeaded();
			grabbedChar->hold(false);
			break;
	}
	calculateAttack(elapsedTime);
}

void EnemyCharacter::moveTowardsTarget(float elapsedTime) {
	if (target.x > getPosition().x) {
		setPosition(Vector2f(getPosition().x + (elapsedTime * speed), getPosition().y));
	}
	else if (target.x < getPosition().x) {
		setPosition(Vector2f(getPosition().x - (elapsedTime * speed), getPosition().y));
	}
	if (target.y > getPosition().y) {
		setPosition(Vector2f(getPosition().x, getPosition().y + (elapsedTime * speed)));
	}
	else if (target.y < getPosition().y) {
		setPosition(Vector2f(getPosition().x, getPosition().y - (elapsedTime * speed)));
	}
}

void EnemyCharacter::handleRunAttackHorizontal(float elapsedTime) {
	// determine whether to send enemy left or run during dash attack
}