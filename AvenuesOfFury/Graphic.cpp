#include "pch.h"
#include "Graphic.h"
#include "SpriteHolder.h"
#include "TextureHolder.h"

void Graphic::setPosition(Vector2f newPosition) {
	position = newPosition;
}

void Graphic::flipHorizontally() {
	sprite.scale(-1, 1);
}

void Graphic::deleteSprite() {
	isActive = false;
	SpriteHolder::deleteSprite(spriteName);
	TextureHolder::deleteTexture(texturePath);
}

void Graphic::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}