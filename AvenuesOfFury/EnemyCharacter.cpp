#include "pch.h"
#include "EnemyCharacter.h"

void EnemyCharacter::update(float elapsedTime, PlayerCharacter** players) {
	if (health <= 0) {
		isActive = false;
		deleteSprite();
		return;
	}
	if (isActive) {
		ActionStruct result = handleAI(elapsedTime, players);
		delegateHandling(elapsedTime, result);
		sprite.setPosition(position);
	}
}

void EnemyCharacter::registerHit(int hp) {
	health -= hp;
}

void EnemyCharacter::handleMove(float elapsedTime, int moveType) {
	renderMove(elapsedTime, moveType);
}

void EnemyCharacter::handleAttack(float elapsedTime, int attackType) {
	renderAttack(elapsedTime, attackType);
}

void EnemyCharacter::handleIdle(float elapsedTime, int idleType) {
	renderIdle(elapsedTime, idleType);
}

void EnemyCharacter::delegateHandling(float elapsedTime, ActionStruct toRender) {
	if (toRender.actionType == Globals::ActionType::MOVE) handleMove(elapsedTime, toRender.action);
	if (toRender.actionType == Globals::ActionType::ATTACK) handleAttack(elapsedTime, toRender.action);
	if (toRender.actionType == Globals::ActionType::IDLE) handleIdle(elapsedTime, toRender.action);
}