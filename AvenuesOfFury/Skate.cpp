#include "pch.h"
#include <fstream>
#include <sstream>
#include "Skate.h"
#include "TextureHolder.h"

using namespace std;

Skate::Skate() {
	sprite = Sprite(TextureHolder::getTexture("graphics/skate_sheet.png"));
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	spriteState = SpriteState::IDLE;
	facingLeft = false;
	facingRight = true;
	speed = 700;
	moveSpriteCycleDown = false;
	idleSpriteCycleDown = false;
	charName = "skate";

	initSprites();
	setIdleSprite(IDLE_1);
	sprite.scale(Vector2f(4.0f, 4.0f));
}

void Skate::setSecondPlayer() {
	speed = 400;
}

void Skate::handleInput() {
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		upPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		upPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		downPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		downPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		leftPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		leftPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		rightPressed = true;
		spriteState = SpriteState::MOVING;
	}
	else {
		rightPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::RControl)) {
		primaryAttackPressed = true;
		spriteState = SpriteState::ATTACKING;
	}
	else {
		primaryAttackPressed = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::RShift)) {
		secondaryAttackPressed = true;
		spriteState = SpriteState::ATTACKING;
	}
	else {
		secondaryAttackPressed = false;
	}
}
