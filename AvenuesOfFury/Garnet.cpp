#include "pch.h"
#include "Garnet.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"

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

	SpriteHolder::initSprites(charName);
	resetMoveFrame(MOVE_1);
	resetAttackFrame(ATTACK_1);
	resetIdleFrame(IDLE_1);

	sprite.scale(Vector2f(4.0f, 4.0f));
	SpriteHolder::setSprite(sprite, charName, "idle", IDLE_1, idleFrame);
}