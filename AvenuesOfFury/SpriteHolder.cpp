#include "pch.h"
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

bool SpriteHolder::getIsStored(string const& charName) {
	auto& storedMap = shInstance->isStoredMap;
	auto storedEntry = storedMap.find(charName);
	if (storedEntry != storedMap.end()) {
		return true;
	}
	return false;
}

void SpriteHolder::setIsStored(string const& charName) {
	shInstance->isStoredMap[charName] = true;
}

Vector2i** SpriteHolder::getMoveSpriteOrigins(string const& charName) {
	auto sMap = shInstance->moveSpriteOriginsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
	else {
		return nullptr;
	}
}

void SpriteHolder::setMoveSpriteOrigins(string const& charName, Vector2i** sprites) {
	shInstance->moveSpriteOriginsMap[charName] = sprites;
}

Vector2i** SpriteHolder::getMoveSpriteBounds(string const& charName) {
	auto sMap = shInstance->moveSpriteBoundsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
	else {
		return nullptr;
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
	else {
		return nullptr;
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
	else {
		return nullptr;
	}
}

void SpriteHolder::setAttackSpriteBounds(string const& charName, Vector2i** sprites) {
	shInstance->attackSpriteBoundsMap[charName] = sprites;
}

Vector2i** SpriteHolder::getIdleSpriteOrigins(string const& charName) {
	auto sMap = shInstance->idleSpriteOriginsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
	else {
		return nullptr;
	}
}

void SpriteHolder::setIdleSpriteOrigins(string const& charName, Vector2i** sprites) {
	shInstance->idleSpriteOriginsMap[charName] = sprites;
}

Vector2i** SpriteHolder::getIdleSpriteBounds(string const& charName) {
	auto sMap = shInstance->idleSpriteBoundsMap;
	auto spriteEntry = sMap.find(charName);
	if (spriteEntry != sMap.end()) {
		return spriteEntry->second;
	}
	else {
		return nullptr;
	}
}

void SpriteHolder::setIdleSpriteBounds(string const& charName, Vector2i** sprites) {
	shInstance->idleSpriteBoundsMap[charName] = sprites;
}

int* SpriteHolder::getMoveTypeMaxFrames(string const& charName) {
	auto mMap = shInstance->moveTypeMaxFramesMap;
	auto countEntry = mMap.find(charName);
	if (countEntry != mMap.end()) {
		return countEntry->second;
	}
	else {
		return nullptr;
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
	else {
		return nullptr;
	}
}

void SpriteHolder::setAttackTypeMaxFrames(string const& charName, int* counts) {
	shInstance->attackTypeMaxFramesMap[charName] = counts;
}

int* SpriteHolder::getIdleTypeMaxFrames(string const& charName) {
	auto mMap = shInstance->idleTypeMaxFramesMap;
	auto countEntry = mMap.find(charName);
	if (countEntry != mMap.end()) {
		return countEntry->second;
	}
	else {
		return nullptr;
	}
}

void SpriteHolder::setIdleTypeMaxFrames(string const& charName, int* counts) {
	shInstance->idleTypeMaxFramesMap[charName] = counts;
}

int* SpriteHolder::getMoveTypeStartFrames(string const& charName) {
	auto mMap = shInstance->moveTypeStartFramesMap;
	auto countEntry = mMap.find(charName);
	if (countEntry != mMap.end()) {
		return countEntry->second;
	}
	else {
		return nullptr;
	}
}

void SpriteHolder::setMoveTypeStartFrames(string const& charName, int* counts) {
	shInstance->moveTypeStartFramesMap[charName] = counts;
}

int* SpriteHolder::getIdleTypeStartFrames(string const& charName) {
	auto mMap = shInstance->idleTypeStartFramesMap;
	auto countEntry = mMap.find(charName);
	if (countEntry != mMap.end()) {
		return countEntry->second;
	}
	else {
		return nullptr;
	}
}

void SpriteHolder::setIdleTypeStartFrames(string const& charName, int* counts) {
	shInstance->idleTypeStartFramesMap[charName] = counts;
}