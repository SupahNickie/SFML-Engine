#include "pch.h"
#include "Skate.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Skate::Skate() {
	MS_PER_FRAME = 75;
	baseSpeedY = -0.00275f * Globals::getResolution().x;
	texturePath = "graphics/characters/skate_sheet.png";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	attackPower = vector<int>{ 60, 60, 60 };
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	spriteState = Globals::ActionType::IDLE;
	facingRight = true;
	baseSpeed = .33 * Globals::getResolution().x;
	speed = baseSpeed;
	health = 4000;
	spriteName = "skate";
	isActive = true;
	runAttackJumps = true;

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(4 * Globals::getScalingFactor(), 4 * Globals::getScalingFactor()));

	currentAction = "idle";
	currentActionType = NORMAL_IDLE;
	resetFrameState();
	render();
}

void Skate::handleInput() {
	Keyboard::isKeyPressed(Keyboard::W) ? upPressed = true : upPressed = false;
	Keyboard::isKeyPressed(Keyboard::S) ? downPressed = true : downPressed = false;
	Keyboard::isKeyPressed(Keyboard::A) ? leftPressed = true : leftPressed = false;
	Keyboard::isKeyPressed(Keyboard::D) ? rightPressed = true : rightPressed = false;
	Keyboard::isKeyPressed(Keyboard::E) ? primaryAttackPressed = true : primaryAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::R) ? secondaryAttackPressed = true : secondaryAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::T) ? specialAttackPressed = true : specialAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::Space) ? jumpPressed = true : jumpPressed = false;
}

// Handle any special logic surrounding the special attack animation (e.g. jumping, zooming around the screen, etc.)
void Skate::handleSpecialAttack() {
	setAttackState("attack", SPECIAL_ATTACK);
	disable(MS_PER_FRAME * SpriteHolder::getMaxFramesForAction(spriteName, "attack", SPECIAL_ATTACK));
}