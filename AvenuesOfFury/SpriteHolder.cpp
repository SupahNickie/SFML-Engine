#include "pch.h";
#include <assert.h>
#include <iostream>
#include "SpriteHolder.h"

using namespace sf;
using namespace std;

SpriteHolder* SpriteHolder::shInstance = nullptr;

SpriteHolder::SpriteHolder() {
	assert(shInstance == nullptr);
	shInstance = this;
}

bool SpriteHolder::isStored(string const& charName) {
	auto& storedMap = shInstance->isStoredMap;
	auto storedEntry = storedMap.find(charName);
	if (storedEntry != storedMap.end()) {
		return true;
	}
	return false;
}

Vector2i** SpriteHolder::getMoveSpriteOrigins(string const& charName) {
	auto sMap = shInstance->moveSpriteOriginsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
}

void SpriteHolder::setMoveSpriteOrigins(string const& charName, Vector2i** sprites) {
	shInstance->isStoredMap[charName] = true;
	shInstance->moveSpriteOriginsMap[charName] = sprites;
}

Vector2i** SpriteHolder::getMoveSpriteBounds(string const& charName) {
	auto sMap = shInstance->moveSpriteBoundsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
}

void SpriteHolder::setMoveSpriteBounds(string const& charName, Vector2i** sprites) {
	shInstance->moveSpriteBoundsMap[charName] = sprites;
}

Vector2i** SpriteHolder::getAttackSpriteOrigins(string const& charName) {
	auto sMap = shInstance->attackSpriteOriginsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
}

void SpriteHolder::setAttackSpriteOrigins(string const& charName, Vector2i** sprites) {
	shInstance->attackSpriteOriginsMap[charName] = sprites;
}

Vector2i** SpriteHolder::getAttackSpriteBounds(string const& charName) {
	auto sMap = shInstance->attackSpriteBoundsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
}

void SpriteHolder::setAttackSpriteBounds(string const& charName, Vector2i** sprites) {
	shInstance->attackSpriteBoundsMap[charName] = sprites;
}

int* SpriteHolder::getMoveTypeMaxFrames(string const& charName) {
	auto mMap = shInstance->moveTypeMaxFramesMap;
	auto countEntry = mMap.find(charName);
	if (countEntry != mMap.end()) {
		return countEntry->second;
	}
}

void SpriteHolder::setMoveTypeMaxFrames(string const& charName, int* counts) {
	shInstance->moveTypeMaxFramesMap[charName] = counts;
}

int* SpriteHolder::getAttackTypeMaxFrames(string const& charName) {
	auto mMap = shInstance->attackTypeMaxFramesMap;
	auto countEntry = mMap.find(charName);
	if (countEntry != mMap.end()) {
		return countEntry->second;
	}
}

void SpriteHolder::setAttackTypeMaxFrames(string const& charName, int* counts) {
	shInstance->attackTypeMaxFramesMap[charName] = counts;
}