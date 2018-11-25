#include "pch.h"
#include "EnemyCharacter.h"

void EnemyCharacter::handleInput() {
	return;
}

void EnemyCharacter::update(float elapsedTime) {
	handleIdle(elapsedTime, IDLE_1);
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