#include "pch.h"
#include "Garnet.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"
#include <iostream>

Garnet::Garnet(vector<Character*> players) : EnemyCharacter(players) {
	MS_PER_FRAME = 100;
	baseSpeedY = -0.00275f * Globals::getResolution().x;
	attackPower[0] = 2000;
	texturePath = "graphics/characters/garnet_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	facingRight = true;
	spriteName = "garnet";

	// randomize some traits
	varianceSpeed = rand() % (int)(.03 * Globals::getResolution().x);
	baseSpeed = .25 * Globals::getResolution().x + varianceSpeed;
	speed = baseSpeed;
	varianceHealth = rand() % 1000;
	health = 5000 + varianceHealth;
	varianceReaction = (rand() % 250) + 50;
	reactionSpeed = (150 + varianceReaction) + 25;
	reactionSpeed -= reactionSpeed % 50;
	maxDecisionSpeed = 2000;

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(4 * Globals::getScalingFactor(), 4 * Globals::getScalingFactor()));

	resetFrameState();
	render();
}

bool Garnet::handleAttacking(float elapsedTime) {
	if (jumping) {
		return jumpAttack(elapsedTime);
	}

	int choice = rand() % 1000;
	if (choice == 0 &&
		abs(position.x - focusChar->getCenter().x) > (0.1 * Globals::getResolution().x) &&
		abs(position.x - focusChar->getCenter().x) < (0.2 * Globals::getResolution().x)
		) return jumpAttack(elapsedTime);

	return attack(elapsedTime, HEAD_ATTACK);
}

void Garnet::handleMoving(float elapsedTime) {
	//moveTowardsFocusChar(elapsedTime);
	predictFocusCharLocation(elapsedTime);
}