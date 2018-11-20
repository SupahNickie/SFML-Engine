#include "pch.h"
#include "Link.h"
#include "TextureHolder.h"

Link::Link() {
	sprite = Sprite(TextureHolder::getTexture("graphics/linksheet.jpg"));
	attackPressed = false;
	spriteState = SpriteState::IDLE;
	setIdleSprite();
	facingLeft = true;
	facingRight = false;
	speed = 300;
	idle1FrameMax = 0;
	attack1FrameMax = 3;
	move1FrameMax = 4;
	move1Frame = 2;
	moveSpriteCycleDown = false;

	initAttackSprites();
	initMoveSprites();
}

void Link::handleInput() {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		attackPressed = true;
		spriteState = SpriteState::ATTACKING;
	}
	else {
		attackPressed = false;
	}

	Keyboard::isKeyPressed(Keyboard::W) ? upPressed = true : upPressed = false;
	Keyboard::isKeyPressed(Keyboard::S) ? downPressed = true : downPressed = false;
	Keyboard::isKeyPressed(Keyboard::A) ? leftPressed = true : leftPressed = false;
	Keyboard::isKeyPressed(Keyboard::D) ? rightPressed = true : rightPressed = false;
}

void Link::handleAttack(float elapsedTime) {
	if (attackDisabled) return;
	timeSinceAttack1Frame += elapsedTime * 1000;
	if (timeSinceAttack1Frame > 50) {
		++attack1Frame;
		timeSinceAttack1Frame = 0;
	}

	if (attack1Frame > attack1FrameMax) {
		setIdleSprite();
		spriteState = SpriteState::IDLE;
		attack1Frame = 0;
		idle1Frame = 0;
		timeSinceAttack1Frame = 0;
		attackPressed = false;
		attackDisabled = true;
	}
	else {
		setAttack1Sprite();
	}
}

void Link::handleMove(float elapsedTime) {
	timeSinceMove1Frame += elapsedTime * 1000;
	if (timeSinceMove1Frame > 50) {
		moveSpriteCycleDown ? --move1Frame : ++move1Frame;
		timeSinceMove1Frame = 0;
	}

	if (move1Frame > move1FrameMax && !moveSpriteCycleDown) {
		moveSpriteCycleDown = true;
		--move1Frame;
		--move1Frame;
	} 
	else if (move1Frame < 0 && moveSpriteCycleDown) {
		moveSpriteCycleDown = false;
		++move1Frame;
		++move1Frame;
	}

	setMove1Sprite();
}

void Link::setIdleSprite() {
	sprite.setTextureRect(IntRect(IDLE_SPRITE_ORIGIN.x, IDLE_SPRITE_ORIGIN.y, IDLE_SPRITE_BOUNDS.x, IDLE_SPRITE_BOUNDS.y));
	sprite.setOrigin(70, 85.5f);
}

void Link::setAttack1Sprite() {
	sprite.setTextureRect(IntRect(attack1SpriteOrigins[attack1Frame].x, attack1SpriteOrigins[attack1Frame].y, attack1SpriteBounds[attack1Frame].x, attack1SpriteBounds[attack1Frame].y));
	sprite.setOrigin(attack1SpriteBounds[attack1Frame].x / 2.0f, attack1SpriteBounds[attack1Frame].y / 2.0f);
}

void Link::setMove1Sprite() {
	sprite.setTextureRect(IntRect(move1SpriteOrigins[move1Frame].x, move1SpriteOrigins[move1Frame].y, move1SpriteBounds[move1Frame].x, move1SpriteBounds[move1Frame].y));
	sprite.setOrigin(move1SpriteBounds[move1Frame].x / 2.0f, move1SpriteBounds[move1Frame].y / 2.0f);
}

void Link::initAttackSprites() {
	delete[] attack1SpriteOrigins;
	attack1SpriteOrigins = new Vector2i[4];
	attack1SpriteOrigins[0] = Vector2i(0, 690);
	attack1SpriteOrigins[1] = Vector2i(230, 680);
	attack1SpriteOrigins[2] = Vector2i(430, 690);
	attack1SpriteOrigins[3] = Vector2i(670, 690);

	delete[] attack1SpriteBounds;
	attack1SpriteBounds = new Vector2i[4];
	attack1SpriteBounds[0] = Vector2i(170, 166);
	attack1SpriteBounds[1] = Vector2i(195, 185);
	attack1SpriteBounds[2] = Vector2i(230, 157);
	attack1SpriteBounds[3] = Vector2i(210, 157);
}

void Link::initMoveSprites() {
	delete[] move1SpriteOrigins;
	move1SpriteOrigins = new Vector2i[5];
	move1SpriteOrigins[0] = Vector2i(30, 0);
	move1SpriteOrigins[1] = Vector2i(207, 0);
	move1SpriteOrigins[2] = Vector2i(395, 0);
	move1SpriteOrigins[3] = Vector2i(573, 0);
	move1SpriteOrigins[4] = Vector2i(750, 0);

	delete[] move1SpriteBounds;
	move1SpriteBounds = new Vector2i[5];
	move1SpriteBounds[0] = Vector2i(150, 180);
	move1SpriteBounds[1] = Vector2i(158, 180);
	move1SpriteBounds[2] = Vector2i(140, 180);
	move1SpriteBounds[3] = Vector2i(149, 180);
	move1SpriteBounds[4] = Vector2i(143, 180);
}