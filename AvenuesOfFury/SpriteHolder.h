#pragma once
#ifndef SPRITE_HOLDER_H
#define SPRITE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Globals.h"

using namespace sf;
using namespace std;

class SpriteHolder {
public:
	SpriteHolder();
	static void initSprites(string const& handlingType, string const& spriteName);
	static void deleteSprite(string const& spriteName);
	static void setSprite(Sprite& sprite, string const& spriteName, string const& actionCategory, int actionType, int actionFrame);
	static vector<int> getDamageFramesForAction(string const& spriteName, string const& actionCategory, int actionType);
	static int getMaxFramesForAction(string const& spriteName, string const& actionCategory, int actionType);
	static int getStartFramesForAction(string const& spriteName, string const& actionCategory, int actionType);
private:
	static SpriteHolder* shInstance;
	enum class HandlingType { CHARACTER, BACKGROUND, NONE };

	unordered_map<string, unordered_map<Globals::ActionType, vector<vector<Vector2i>>>> spriteOriginsMap;
	unordered_map<string, unordered_map<Globals::ActionType, vector<vector<Vector2i>>>> spriteBoundsMap;
	unordered_map<string, unordered_map<Globals::ActionType, vector<vector<int>>>> spriteDamageFramesMap;
	unordered_map<string, unordered_map<Globals::ActionType, vector<int>>> spriteMaxFrameMap;
	unordered_map<string, unordered_map<Globals::ActionType, vector<int>>> spriteStartFrameMap;
	unordered_map<string, bool> isStoredMap;

	static void handleCharacterLine(vector<string> line, string* currentlySetting, unsigned int* numberOfActions, string spriteName, int* outIndex, int* inIndex);
	static void handleBackgroundLine(vector<string> line, string* currentlySetting, unsigned int* numberOfActions, string spriteName, int* outIndex, int* inIndex);
	static void initActionSprites(string const& spriteName, Globals::ActionType action, unsigned int moveCount);
	static void addActionSpriteFrames(string const& spriteName, Globals::ActionType action, int numberOfActions, int numberOfFrames, int startFrame, vector<int> &damageFrames);
	static void setActionSpriteFrames(string const& spriteName, Globals::ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	static bool getIsStored(string const& spriteName);
	static void setIsStored(string const& spriteName);
	static HandlingType handlingStringToEnum(string const& handling);
};

#endif 