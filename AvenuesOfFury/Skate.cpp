#include "pch.h"
#include "Skate.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Skate::Skate() {
	MS_PER_FRAME = 100;
	texturePath = "graphics/characters/skate_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	attackPower = vector<int>(1);
	attackPower[0] = 600;
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	spriteState = Globals::ActionType::IDLE;
	facingLeft = false;
	facingRight = true;
	speed = .33 * Globals::getResolution().x;
	health = 4000;
	spriteName = "skate";
	isActive = true;

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(4 * Globals::getScalingFactor(), 4 * Globals::getScalingFactor()));

	animationCycle = { {"move", false}, {"attack", false}, {"idle", false}, {"injure", false} };
	currentAction = "idle";
	currentActionType = IDLE_1;
	resetFrameState();
	render();
}

void Skate::handleInput() {
	Keyboard::isKeyPressed(Keyboard::Up) ? upPressed = true : upPressed = false;
	Keyboard::isKeyPressed(Keyboard::Down) ? downPressed = true : downPressed = false;
	Keyboard::isKeyPressed(Keyboard::Left) ? leftPressed = true : leftPressed = false;
	Keyboard::isKeyPressed(Keyboard::Right) ? rightPressed = true : rightPressed = false;
	Keyboard::isKeyPressed(Keyboard::RControl) ? primaryAttackPressed = true : primaryAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::RShift) ? secondaryAttackPressed = true : secondaryAttackPressed = false;
}