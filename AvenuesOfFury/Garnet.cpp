#include "pch.h"
#include "Garnet.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"
#include <iostream>

Garnet::Garnet(vector<PlayerCharacter*> players) {
	MS_PER_FRAME = 100;
	attackPower = vector<int>(1);
	attackPower[0] = 2000;
	texturePath = "graphics/characters/garnet_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	spriteState = Globals::ActionType::IDLE;
	facingLeft = false;
	facingRight = true;
	spriteName = "garnet";
	isActive = true;

	// randomize some traits
	varianceSpeed = rand() % (int)(.03 * Globals::getResolution().x);
	speed = .25 * Globals::getResolution().x + varianceSpeed;
	varianceHealth = rand() % 1000;
	health = 5000 + varianceHealth;
	varianceAggression = rand() % 100;
	aggression = 200 + varianceAggression;
	varianceReaction = (rand() % 250) + 50;
	reactionSpeed = (150 + varianceReaction) + 25;
	reactionSpeed -= reactionSpeed % 50;

	focusChar = players[1];

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(4 * Globals::getScalingFactor(), 4 * Globals::getScalingFactor()));

	animationCycle = false;
	currentAction = "idle";
	currentActionType = IDLE_HAIR;
	resetFrameState();
	render();
}

void Garnet::handleAI(float elapsedTime, vector<PlayerCharacter*> players) {
	EnemyCharacter::handleAI(elapsedTime, players);
}