#include "pch.h"
#include "Graphic.h"

void Graphic::setPosition(Vector2f newPosition) {
	position = newPosition;
}

void Graphic::flipHorizontally() {
	sprite.scale(-1, 1);
}

void Graphic::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}