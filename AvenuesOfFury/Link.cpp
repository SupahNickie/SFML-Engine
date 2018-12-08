#include "pch.h"
#include "Link.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Link::Link() {
	texturePath = "graphics/characters/link_sheet.jpg";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	spriteState = SpriteState::IDLE;
	facingLeft = true;
	facingRight = false;
	speed = 700;
	animationCycle = true;
	moveSpriteCycleDown = false;
	attackSpriteCycleDown = false;
	idleSpriteCycleDown = false;
	spriteName = "link";
	isActive = true;

	SpriteHolder::initSprites("character", spriteName);
	resetMoveFrame(MOVE_1);
	resetAttackFrame(ATTACK_1);
	resetIdleFrame(IDLE_1);

	SpriteHolder::setSprite(sprite, spriteName, "idle", IDLE_1, idleFrame);
}

void Link::setSecondPlayer() {
	speed = 400;
}

void Link::handleInput() {
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		upPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		upPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		downPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		downPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		leftPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		leftPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::D)) {
		rightPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		rightPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		primaryAttackPressed = true;
		spriteState = SpriteState::ATTACKING;
	}
	else {
		primaryAttackPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::E)) {
		secondaryAttackPressed = true;
		spriteState = SpriteState::ATTACKING;
	}
	else {
		secondaryAttackPressed = false;
	}
}
