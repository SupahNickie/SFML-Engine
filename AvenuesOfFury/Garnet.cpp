#include "pch.h"
#include "Garnet.h"
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

	SpriteHolder::initSprites("character", spriteName);
	resetMoveFrame(MOVE_1);
	resetAttackFrame(ATTACK_1);
	resetIdleFrame(IDLE_1);

	sprite.scale(Vector2f(4.0f, 4.0f));
	SpriteHolder::setSprite(sprite, spriteName, "idle", IDLE_1, idleFrame);
}

void Garnet::update(float elapsedTime) {
	handleIdle(elapsedTime, IDLE_1);

	sprite.setPosition(position);
}