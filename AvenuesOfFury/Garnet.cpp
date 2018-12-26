#include "pch.h"
#include "Garnet.h"
#include "Globals.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Garnet::Garnet(PlayerCharacter** players) {
	MS_PER_FRAME = 100;
	texturePath = "graphics/characters/garnet_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	spriteState = Globals::ActionType::IDLE;
	facingLeft = false;
	facingRight = true;
	speed = 500;
	spriteName = "garnet";
	isActive = true;

	health = 500;
	aggression = 1500;
	defense = 200;

	focusChar = players[0];

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(4.0f, 4.0f));

	animationCycle = false;
	currentAction = "idle";
	currentActionType = IDLE_HAIR;
	resetFrameState();
	render();
}

void Garnet::handleAI(float elapsedTime, PlayerCharacter** players) {
	if (currentActionDone) {
		spriteState = Globals::ActionType::IDLE;
		currentAction = "idle";
		currentActionType = IDLE_HAIR;
		resetFrameState();
	}
}