#include "pch.h"
#include "EnemyCharacter.h"

void EnemyCharacter::update(float elapsedTime, PlayerCharacter** players) {
	if (health <= 0) {
		isActive = false;
		deleteSprite();
		return;
	}
	if (isActive) {
		updateFrameState(elapsedTime);
		handleAI(elapsedTime, players);
		sprite.setPosition(position);
		render();
	}
}

void EnemyCharacter::registerHit(int hp, float elapsedTime) {
	if (spriteState != Globals::ActionType::INJURE) {
		spriteState = Globals::ActionType::INJURE;
		currentAction = "injure";
		currentActionType = INJURE_1;
		resetFrameState();
	}
	health -= hp;
}