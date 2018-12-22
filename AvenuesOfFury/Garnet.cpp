#include "pch.h"
#include "Garnet.h"
#include "Globals.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Garnet::Garnet() {
	MS_PER_FRAME = 100;
	texturePath = "graphics/characters/garnet_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	spriteState = SpriteState::IDLE;
	facingLeft = false;
	facingRight = true;
	speed = 500;
	animationCycle = false;
	moveSpriteCycleDown = false;
	attackSpriteCycleDown = false;
	idleSpriteCycleDown = false;
	spriteName = "garnet";
	isActive = true;

	health = 500;
	aggression = 1500;
	defense = 200;

	SpriteHolder::initSprites("character", spriteName);
	resetMoveFrame(MOVE_WALK);
	resetAttackFrame(ATTACK_SLAP);
	resetIdleFrame(IDLE_HAIR);

	sprite.scale(Vector2f(4.0f, 4.0f));
	SpriteHolder::setSprite(sprite, spriteName, "idle", IDLE_HAIR, idleFrame);
}

ActionStruct Garnet::handleAI(float elapsedTime, PlayerCharacter** players) {
	ActionStruct output;

	

	output.actionType = Globals::ActionType::IDLE;
	output.action = IDLE_HAIR;


	return output;
}