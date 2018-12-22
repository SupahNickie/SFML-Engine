#include "pch.h"
#include "Graphic.h"
#include "SpriteHolder.h"
#include "TextureHolder.h"

using namespace sf;

void Graphic::flipHorizontally() {
	sprite.scale(-1, 1);
}

void Graphic::setPosition(Vector2f newPosition) {
	position = newPosition;
}

FloatRect Graphic::getPosition() {
	return sprite.getGlobalBounds();
}

Vector2f Graphic::getCenter() {
	return position;
}

void Graphic::deleteSprite() {
	isActive = false;
	SpriteHolder::deleteSprite(spriteName);
	TextureHolder::deleteTexture(texturePath);
}

void Graphic::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}