#include "pch.h"
#include <fstream>
#include <sstream>
#include "Link.h"
#include "TextureHolder.h"

using namespace std;

Link::Link() {
	sprite = Sprite(TextureHolder::getTexture("graphics/linksheet.jpg"));
	attackPressed = false;
	spriteState = SpriteState::IDLE;
	setIdleSprite();
	facingLeft = true;
	facingRight = false;
	speed = 700;
	idleFrameMax = 0;
	moveSpriteCycleDown = false;
	charName = "link";

	initSprites();
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
		attackPressed = true;
		spriteState = SpriteState::ATTACKING;
	}
	else {
		attackPressed = false;
	}
}

void Link::setIdleSprite() {
	sprite.setTextureRect(IntRect(IDLE_SPRITE_ORIGIN.x, IDLE_SPRITE_ORIGIN.y, IDLE_SPRITE_BOUNDS.x, IDLE_SPRITE_BOUNDS.y));
	sprite.setOrigin(70, 85.5f);
	resetMoveFrame(MOVE_WALK);
}

void Link::resetMoveFrame(int moveType) {
	if (moveType == MOVE_WALK) moveFrame = 2;
}
