#include "pch.h"
#include <iostream>
#include "EnemyCharacter.h"

void EnemyCharacter::update(float elapsedTime) {
	handleIdle(elapsedTime, IDLE_1);

	sprite.setPosition(position);
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