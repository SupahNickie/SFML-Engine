#include "pch.h"
#include "Garnet.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"
#include <iostream>

Garnet::Garnet(vector<PlayerCharacter*> players) : EnemyCharacter(players) {
	MS_PER_FRAME = 100;
	attackPower[0] = 2000;
	texturePath = "graphics/characters/garnet_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	facingLeft = false;
	facingRight = true;
	spriteName = "garnet";

	// randomize some traits
	varianceSpeed = rand() % (int)(.03 * Globals::getResolution().x);
	speed = .25 * Globals::getResolution().x + varianceSpeed;
	varianceHealth = rand() % 1000;
	health = 5000 + varianceHealth;
	varianceReaction = (rand() % 250) + 50;
	reactionSpeed = (150 + varianceReaction) + 25;
	reactionSpeed -= reactionSpeed % 50;
	maxDecisionSpeed = 2000;

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(4 * Globals::getScalingFactor(), 4 * Globals::getScalingFactor()));

	animationCycle = { {"move", false}, {"attack", false}, {"idle", false}, {"injure", false} };
	resetFrameState();
	render();
}

bool Garnet::handleAttacking(float elapsedTime) {
	return basicAttack(elapsedTime);
}