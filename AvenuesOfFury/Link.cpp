#include "pch.h"
#include "Link.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Link::Link() {
	setupGraphic("graphics/characters/link_sheet.jpg", "link");
	SpriteHolder::initSprites("character", getSpriteName());
	changeScale(Vector2f(Globals::getScalingFactor(), Globals::getScalingFactor()));
	setAnimationCycle({ {"move", true} });
	facingRight = false;

	attackPower = vector<int>{ 1000, 4000 };
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	baseSpeed = .33 * Globals::getResolution().x;
	speed = baseSpeed;

	currentAction = "idle";
	spriteState = Globals::ActionType::IDLE;
	currentActionType = NORMAL_IDLE;
	resetFrameState();
	render();
}

void Link::handleInput() {
	Keyboard::isKeyPressed(Keyboard::Up) ? upPressed = true : upPressed = false;
	Keyboard::isKeyPressed(Keyboard::Down) ? downPressed = true : downPressed = false;
	Keyboard::isKeyPressed(Keyboard::Left) ? leftPressed = true : leftPressed = false;
	Keyboard::isKeyPressed(Keyboard::Right) ? rightPressed = true : rightPressed = false;
	Keyboard::isKeyPressed(Keyboard::RControl) ? primaryAttackPressed = true : primaryAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::RShift) ? secondaryAttackPressed = true : secondaryAttackPressed = false;
}

// Handle any special logic surrounding the special attack animation (e.g. jumping, zooming around the screen, etc.)
void Link::handleSpecialAttack() {
	return;
}