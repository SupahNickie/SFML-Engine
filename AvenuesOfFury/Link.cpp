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
	speed = 700;
	idleFrameMax = 0;
	moveSpriteCycleDown = false;

	initAttackSprites();
	initMoveSprites();
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

void Link::initAttackSprites() {
	delete[] attackSpriteOrigins;
	attackSpriteOrigins = new Vector2i*[1];
	attackSpriteOrigins[0] = new Vector2i[4];
	attackSpriteOrigins[0][0] = Vector2i(0, 690);
	attackSpriteOrigins[0][1] = Vector2i(230, 680);
	attackSpriteOrigins[0][2] = Vector2i(430, 690);
	attackSpriteOrigins[0][3] = Vector2i(670, 690);

	delete[] attackSpriteBounds;
	attackSpriteBounds = new Vector2i*[1];
	attackSpriteBounds[0] = new Vector2i[4];
	attackSpriteBounds[0][0] = Vector2i(170, 166);
	attackSpriteBounds[0][1] = Vector2i(195, 185);
	attackSpriteBounds[0][2] = Vector2i(230, 157);
	attackSpriteBounds[0][3] = Vector2i(210, 157);

	delete[] attackTypeMaxFrames;
	attackTypeMaxFrames = new int[1];
	attackTypeMaxFrames[0] = 3;

	resetAttackFrame();
}

void Link::initMoveSprites() {
	delete[] moveSpriteOrigins;
	moveSpriteOrigins = new Vector2i*[1];
	moveSpriteOrigins[0] = new Vector2i[5];
	moveSpriteOrigins[0][0] = Vector2i(30, 0);
	moveSpriteOrigins[0][1] = Vector2i(207, 0);
	moveSpriteOrigins[0][2] = Vector2i(395, 0);
	moveSpriteOrigins[0][3] = Vector2i(573, 0);
	moveSpriteOrigins[0][4] = Vector2i(750, 0);

	delete[] moveSpriteBounds;
	moveSpriteBounds = new Vector2i*[1];
	moveSpriteBounds[0] = new Vector2i[5];
	moveSpriteBounds[0][0] = Vector2i(150, 180);
	moveSpriteBounds[0][1] = Vector2i(158, 180);
	moveSpriteBounds[0][2] = Vector2i(140, 180);
	moveSpriteBounds[0][3] = Vector2i(149, 180);
	moveSpriteBounds[0][4] = Vector2i(143, 180);
	
	delete[] moveTypeMaxFrames;
	moveTypeMaxFrames = new int[1];
	moveTypeMaxFrames[0] = 4;

	resetMoveFrame(MOVE_WALK);
}