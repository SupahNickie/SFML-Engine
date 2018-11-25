#include "pch.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "SpriteHolder.h"

using namespace sf;
using namespace std;

SpriteHolder* SpriteHolder::shInstance = nullptr;

SpriteHolder::SpriteHolder() {
	assert(shInstance == nullptr);
	shInstance = this;
}

void SpriteHolder::initSprites(string const& charName) {
	if (SpriteHolder::getIsStored(charName)) return;

	string currentlySetting = "";
	unsigned int numberOfActions = 0;
	int outIndex = 0;
	int inIndex = 0;
	unsigned int numberOfFrames = 0;
	unsigned int startFrame = 0;

	stringstream ss;
	ss << "graphics/" << charName << "_coords.txt";

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

		if (line[0] == "move" || line[0] == "attack" || line[0] == "idle") {
			currentlySetting = line[0];
			numberOfActions = stoi(line[1]);
			initActionSprites(charName, actionStringToEnum(currentlySetting), numberOfActions);
		}
		else if (line[0] == "frames") {
			inIndex = 0;
			outIndex = 0;
			numberOfActions = 0;
			for (unsigned int i = 1; i < line.size(); ++numberOfActions) {
				numberOfFrames = stoi(line[i]);
				++i;
				startFrame = stoi(line[i]);
				++i;
				addActionSpriteFrames(charName, actionStringToEnum(currentlySetting), numberOfActions, numberOfFrames, startFrame);
			}
		}
		else if (line[0] == "skip") {
			inIndex = 0;
			++outIndex;
		}
		else {
			setActionSpriteFrames(
				charName,
				actionStringToEnum(currentlySetting),
				outIndex,
				inIndex,
				Vector2i(stoi(line[0]), stoi(line[1])),
				Vector2i(stoi(line[2]), stoi(line[3]))
			);
			++inIndex;
		}
	}
	inputFile.close();

	setIsStored(charName);
}

void SpriteHolder::setSprite(Sprite& sprite, string const& charName, string const& actionCategory, int actionType, int actionFrame) {
	Vector2i originPos = shInstance->charOriginsMap[charName][actionStringToEnum(actionCategory)][actionType][actionFrame];
	Vector2i boundPos = shInstance->charBoundsMap[charName][actionStringToEnum(actionCategory)][actionType][actionFrame];

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

int SpriteHolder::getMaxFramesForAction(string const& charName, string const& actionCategory, int actionType) {
	return shInstance->charMaxFrameMap[charName][actionStringToEnum(actionCategory)][actionType];
}

int SpriteHolder::getStartFramesForAction(string const& charName, string const& actionCategory, int actionType) {
	return shInstance->charStartFrameMap[charName][actionStringToEnum(actionCategory)][actionType];
}

void SpriteHolder::initActionSprites(string const& charName, ActionType action, unsigned int actionCount) {
	shInstance->charOriginsMap[charName][action] = new Vector2i*[actionCount];
	shInstance->charBoundsMap[charName][action] = new Vector2i*[actionCount];
	shInstance->charMaxFrameMap[charName][action] = new int[actionCount];
	shInstance->charStartFrameMap[charName][action] = new int[actionCount];
}

void SpriteHolder::addActionSpriteFrames(string const& charName, ActionType action, int numberOfActions, int numberOfFrames, int startFrame) {
	shInstance->charOriginsMap[charName][action][numberOfActions] = new Vector2i[numberOfFrames];
	shInstance->charBoundsMap[charName][action][numberOfActions] = new Vector2i[numberOfFrames];
	shInstance->charMaxFrameMap[charName][action][numberOfActions] = numberOfFrames - 1;
	shInstance->charStartFrameMap[charName][action][numberOfActions] = startFrame;
}

void SpriteHolder::setActionSpriteFrames(string const& charName, ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound) {
	shInstance->charOriginsMap[charName][action][outIndex][inIndex] = origin;
	shInstance->charBoundsMap[charName][action][outIndex][inIndex] = bound;
}

bool SpriteHolder::getIsStored(string const& charName) {
	auto storedEntry = shInstance->isStoredMap.find(charName);
	if (storedEntry != shInstance->isStoredMap.end()) {
		return true;
	}
	return false;
}

void SpriteHolder::setIsStored(string const& charName) {
	shInstance->isStoredMap[charName] = true;
}

SpriteHolder::ActionType SpriteHolder::actionStringToEnum(string const& action) {
	if (action == "move") return ActionType::MOVE;
	if (action == "attack") return ActionType::ATTACK;
	if (action == "idle") return ActionType::IDLE;
}