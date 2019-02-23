#include "pch.h"
#include "Graphic.h"
#include "SpriteHolder.h"
#include "TextureHolder.h"

using namespace sf;

void Graphic::flipHorizontally() {
	sprite.scale(-1, 1);
}

void Graphic::setAnimationCycle(unordered_map<string, bool> cycle) {
	animationCycle = cycle;
}

unordered_map<string, bool> Graphic::getAnimationCycle() {
	return animationCycle;
}

void Graphic::setTexturePath(string const& newTexturePath) {
	texturePath = newTexturePath;
}

void Graphic::setSprite(Sprite const& newSprite) {
	sprite = newSprite;
}

Sprite* Graphic::getSprite() {
	return &sprite;
}

void Graphic::deleteSprite() {
	SpriteHolder::deleteSprite(spriteName);
	TextureHolder::deleteTexture(texturePath);
}

void Graphic::setPosition(Vector2f newPosition) {
	position = newPosition;
	sprite.setPosition(position);
}

Vector2f Graphic::getPosition() {
	return position;
}

FloatRect Graphic::getBounds() {
	return sprite.getGlobalBounds();
}

void Graphic::setSpriteName(string const& newSpriteName) {
	spriteName = newSpriteName;
}

string Graphic::getSpriteName() {
	return spriteName;
}

void Graphic::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}