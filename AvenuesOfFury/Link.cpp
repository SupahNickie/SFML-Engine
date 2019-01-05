#include "pch.h"
#include "Link.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

Link::Link() {
	texturePath = "graphics/characters/link_sheet.jpg";
	sprite = Sprite(TextureHolder::getTexture(texturePath));
	attackPower = vector<int>(2);
	attackPower[0] = 1000;
	attackPower[1] = 4000;
	primaryAttackPressed = false;
	secondaryAttackPressed = false;
	spriteState = Globals::ActionType::IDLE;
	facingLeft = true;
	facingRight = false;
	baseSpeed = .33 * Globals::getResolution().x;
	speed = baseSpeed;

	spriteName = "link";
	isActive = true;

	SpriteHolder::initSprites("character", spriteName);
	sprite.scale(Vector2f(Globals::getScalingFactor(), Globals::getScalingFactor()));
	
	animationCycle = { {"move", true}, {"attack", false}, {"idle", false}, {"injure", false} };
	currentAction = "idle";
	currentActionType = IDLE_1;
	resetFrameState();
	render();
}

void Link::handleInput() {
	Keyboard::isKeyPressed(Keyboard::W) ? upPressed = true : upPressed = false;
	Keyboard::isKeyPressed(Keyboard::S) ? downPressed = true : downPressed = false;
	Keyboard::isKeyPressed(Keyboard::A) ? leftPressed = true : leftPressed = false;
	Keyboard::isKeyPressed(Keyboard::D) ? rightPressed = true : rightPressed = false;
	Keyboard::isKeyPressed(Keyboard::Space) ? primaryAttackPressed = true : primaryAttackPressed = false;
	Keyboard::isKeyPressed(Keyboard::E) ? secondaryAttackPressed = true : secondaryAttackPressed = false;
}