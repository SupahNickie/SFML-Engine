#include "pch.h"
#include "Link.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Link::Link() {
	texturePath = "graphics/characters/link_sheet.jpg";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	spriteState = Globals::ActionType::IDLE;
	facingLeft = true;
	facingRight = false;
	speed = 700;
	spriteName = "link";
	isActive = true;

	SpriteHolder::initSprites("character", spriteName);
	
	animationCycle = true;
	currentAction = "idle";
	currentActionType = IDLE_1;
	resetFrameState();
	render();
}

void Link::setSecondPlayer() {
	speed = 400;
}

void Link::handleInput() {
	Keyboard::isKeyPressed(Keyboard::W) ? upPressed = true : upPressed = false;
	Keyboard::isKeyPressed(Keyboard::S) ? downPressed = true : downPressed = false;
	Keyboard::isKeyPressed(Keyboard::A) ? leftPressed = true : leftPressed = false;
	Keyboard::isKeyPressed(Keyboard::D) ? rightPressed = true : rightPressed = false;
	Keyboard::isKeyPressed(Keyboard::Space) ? primaryAttackPressed = true : primaryAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::E) ? secondaryAttackPressed = true : secondaryAttackPressed = false;
}