#include "pch.h"
#include "Garnet.h"
#include "TextureHolder.h"

Garnet::Garnet() {
	MS_PER_FRAME = 100;
	sprite = Sprite(TextureHolder::getTexture("graphics/garnet_sheet.png"));
	spriteState = SpriteState::IDLE;
	facingLeft = false;
	facingRight = true;
	speed = 500;
	animationCycle = false;
	moveSpriteCycleDown = false;
	attackSpriteCycleDown = false;
	idleSpriteCycleDown = false;
	charName = "garnet";

	initSprites();
	sprite.scale(Vector2f(4.0f, 4.0f));
}