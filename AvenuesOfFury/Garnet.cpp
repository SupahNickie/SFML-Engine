#include "pch.h"
#include "Garnet.h"
#include "SpriteHolder.h"
#include <iostream>

Garnet::Garnet(vector<Character*> players) : EnemyCharacter(players) {
	MS_PER_FRAME = 100;
	setupGraphic("graphics/characters/garnet_sheet.png", "garnet");
	SpriteHolder::initSprites("character", getSpriteName());
	changeScale(Vector2f(4 * Globals::getScalingFactor(), 4 * Globals::getScalingFactor()));
	facingRight = true;

	// randomize some traits
	baseSpeedY = -0.00275f * Globals::getResolution().x;
	attackPower[0] = 2000;
	varianceSpeed = rand() % (int)(.03 * Globals::getResolution().x);
	baseSpeed = .25 * Globals::getResolution().x + varianceSpeed;
	speed = baseSpeed;
	varianceHealth = rand() % 1000;
	health = 5000 + varianceHealth;
	varianceReaction = (rand() % 250) + 50;
	reactionSpeed = (150 + varianceReaction) + 25;
	reactionSpeed -= reactionSpeed % 50;
	maxDecisionSpeed = 2000;

	resetFrameState();
	render();
}

bool Garnet::handleAttacking(float elapsedTime) {
	if (jumping) return jumpAttack(elapsedTime);
	if (grabbing) return grabAttack(elapsedTime);

	int choice = rand() % 1000;
	if (choice == 0 &&
		abs(getPosition().x - focusChar->getPosition().x) > (0.1 * Globals::getResolution().x) &&
		abs(getPosition().x - focusChar->getPosition().x) < (0.2 * Globals::getResolution().x)
		) return jumpAttack(elapsedTime);

	if (choice == 1 &&
		abs(getPosition().x - focusChar->getPosition().x) < (0.1 * Globals::getResolution().x)
		) return grabAttack(elapsedTime);

	return attack(elapsedTime, HEAD_ATTACK);
}

void Garnet::handleMoving(float elapsedTime) {
	//moveTowardsFocusChar(elapsedTime);
	predictFocusCharLocation(elapsedTime);
}