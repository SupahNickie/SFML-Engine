#include "pch.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Globals.h"
#include "SpriteHolder.h"

using namespace sf;
using namespace std;

SpriteHolder* SpriteHolder::shInstance = nullptr;

SpriteHolder::SpriteHolder() {
	assert(shInstance == nullptr);
	shInstance = this;
}

void SpriteHolder::initSprites(string const& handlingType, string const& spriteName) {
	if (SpriteHolder::getIsStored(spriteName)) return;

	string currentlySetting = "";
	unsigned int numberOfActions = 0;
	int outIndex = 0;
	int inIndex = 0;

	stringstream ss;
	ss << handlingType << "s/" << spriteName << "_coords.txt";

	string s;
	const string delimiter = ",";
	ifstream inputFile(ss.str());
	while (getline(inputFile, s)) {
		size_t pos = 0;
		string current = "";
		vector<string> line{};

		while ((pos = s.find(delimiter)) != string::npos) {
			current = s.substr(0, pos);
			line.push_back(current);
			s.erase(0, pos + delimiter.length());
		}
		line.push_back(s);

		if (handlingStringToEnum(handlingType) == HandlingType::CHARACTER) {
			handleCharacterLine(line, &currentlySetting, &numberOfActions, spriteName, &outIndex, &inIndex);
		}
		else if (handlingStringToEnum(handlingType) == HandlingType::BACKGROUND) {
			handleBackgroundLine(line, &currentlySetting, &numberOfActions, spriteName, &outIndex, &inIndex);
		}
		else {
			break;
		}
	}
	inputFile.close();

	setIsStored(spriteName);
}


void SpriteHolder::deleteSprite(string const& spriteName) {
	shInstance->spriteOriginsMap.erase(spriteName);
	shInstance->spriteBoundsMap.erase(spriteName);
	shInstance->spriteDamageFramesMap.erase(spriteName);
	shInstance->spriteMaxFrameMap.erase(spriteName);
	shInstance->spriteStartFrameMap.erase(spriteName);
}


void SpriteHolder::handleCharacterLine(vector<string> line, string* currentlySetting, unsigned int* numberOfActions, string spriteName, int* outIndex, int* inIndex) {
	if (Globals::actionStringToEnum(line[0]) != Globals::ActionType::NONE) {
		*currentlySetting = line[0];
		*numberOfActions = stoi(line[1]);
		initActionSprites(spriteName, Globals::actionStringToEnum(*currentlySetting), *numberOfActions);
	}
	else if (line[0] == "frames") {
		*inIndex = 0;
		*outIndex = 0;
		*numberOfActions = 0;
		for (unsigned int i = 1; i < line.size(); ++*numberOfActions) {
			unsigned int numberOfFrames = 0;
			unsigned int startFrame = 0;
			vector<int> damageFrames;

			numberOfFrames = stoi(line[i]);
			++i;
			startFrame = stoi(line[i]);
			++i;
			while (line[i] != ";") {
				damageFrames.push_back(stoi(line[i]));
				++i;
			}
			++i;
			addActionSpriteFrames(spriteName, Globals::actionStringToEnum(*currentlySetting), *numberOfActions, numberOfFrames, startFrame, damageFrames);
		}
	}
	else if (line[0] == "skip") {
		*inIndex = 0;
		++*outIndex;
	}
	else {
		setActionSpriteFrames(
			spriteName,
			Globals::actionStringToEnum(*currentlySetting),
			*outIndex,
			*inIndex,
			Vector2i(stoi(line[0]), stoi(line[1])),
			Vector2i(stoi(line[2]), stoi(line[3]))
		);
		++*inIndex;
	}
}

void SpriteHolder::handleBackgroundLine(vector<string> line, string* currentlySetting, unsigned int* numberOfActions, string spriteName, int* outIndex, int* inIndex) {
	
}

void SpriteHolder::setSprite(Sprite& sprite, string const& spriteName, string const& actionCategory, int actionType, int actionFrame) {
	Vector2i originPos = shInstance->spriteOriginsMap[spriteName][Globals::actionStringToEnum(actionCategory)][actionType][actionFrame];
	Vector2i boundPos = shInstance->spriteBoundsMap[spriteName][Globals::actionStringToEnum(actionCategory)][actionType][actionFrame];

	sprite.setTextureRect(
		IntRect(
			originPos.x,
			originPos.y,
			boundPos.x,
			boundPos.y
		)
	);
	sprite.setOrigin(
		boundPos.x / 2.0f,
		boundPos.y / 2.0f
	);
}

vector<int> SpriteHolder::getDamageFramesForAction(string const& spriteName, string const& actionCategory, int actionType) {
	return shInstance->spriteDamageFramesMap[spriteName][Globals::actionStringToEnum(actionCategory)][actionType];
}

int SpriteHolder::getMaxFramesForAction(string const& spriteName, string const& actionCategory, int actionType) {
	return shInstance->spriteMaxFrameMap[spriteName][Globals::actionStringToEnum(actionCategory)][actionType];
}

int SpriteHolder::getStartFramesForAction(string const& spriteName, string const& actionCategory, int actionType) {
	return shInstance->spriteStartFrameMap[spriteName][Globals::actionStringToEnum(actionCategory)][actionType];
}

void SpriteHolder::initActionSprites(string const& spriteName, Globals::ActionType action, unsigned int actionCount) {
	shInstance->spriteOriginsMap[spriteName][action] = vector<vector<Vector2i>>(actionCount);
	shInstance->spriteBoundsMap[spriteName][action] = vector<vector<Vector2i>>(actionCount);
	shInstance->spriteDamageFramesMap[spriteName][action] = vector<vector<int>>(actionCount);
	shInstance->spriteMaxFrameMap[spriteName][action] = vector<int>(actionCount);
	shInstance->spriteStartFrameMap[spriteName][action] = vector<int>(actionCount);
}

void SpriteHolder::addActionSpriteFrames(string const& spriteName, Globals::ActionType action, int numberOfActions, int numberOfFrames, int startFrame, vector<int> &damageFrames) {
	shInstance->spriteOriginsMap[spriteName][action][numberOfActions] = vector<Vector2i>(numberOfFrames);
	shInstance->spriteBoundsMap[spriteName][action][numberOfActions] = vector<Vector2i>(numberOfFrames);
	shInstance->spriteDamageFramesMap[spriteName][action][numberOfActions] = damageFrames;
	shInstance->spriteMaxFrameMap[spriteName][action][numberOfActions] = numberOfFrames - 1;
	shInstance->spriteStartFrameMap[spriteName][action][numberOfActions] = startFrame;
}

void SpriteHolder::setActionSpriteFrames(string const& spriteName, Globals::ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound) {
	shInstance->spriteOriginsMap[spriteName][action][outIndex][inIndex] = origin;
	shInstance->spriteBoundsMap[spriteName][action][outIndex][inIndex] = bound;
}

bool SpriteHolder::getIsStored(string const& spriteName) {
	auto storedEntry = shInstance->isStoredMap.find(spriteName);
	if (storedEntry != shInstance->isStoredMap.end()) {
		return true;
	}
	return false;
}

void SpriteHolder::setIsStored(string const& spriteName) {
	shInstance->isStoredMap[spriteName] = true;
}

SpriteHolder::HandlingType SpriteHolder::handlingStringToEnum(string const& handling) {
	if (handling == "character") return HandlingType::CHARACTER;
	if (handling == "background") return HandlingType::BACKGROUND;
	return HandlingType::NONE;
}