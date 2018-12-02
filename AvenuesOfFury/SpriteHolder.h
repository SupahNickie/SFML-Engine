#pragma once
#ifndef SPRITE_HOLDER_H
#define SPRITE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class SpriteHolder {
private:
	static SpriteHolder* shInstance;
	enum class ActionType { MOVE, ATTACK, IDLE, NONE };
	enum class HandlingType { CHARACTER, BACKGROUND, NONE };

	map<string, map<ActionType, Vector2i**>> spriteOriginsMap;
	map<string, map<ActionType, Vector2i**>> spriteBoundsMap;
	map<string, map<ActionType, int*>> spriteMaxFrameMap;
	map<string, map<ActionType, int*>> spriteStartFrameMap;
	map<string, bool> isStoredMap;

	static void handleCharacterLine(vector<string> line, string* currentlySetting, unsigned int* numberOfActions, string spriteName, int* outIndex, int* inIndex, unsigned int* numberOfFrames, unsigned int* startFrame);
	static void handleBackgroundLine(vector<string> line, string* currentlySetting, unsigned int* numberOfActions, string spriteName, int* outIndex, int* inIndex, unsigned int* numberOfFrames, unsigned int* startFrame);
	static void initActionSprites(string const& spriteName, ActionType action, unsigned int moveCount);
	static void addActionSpriteFrames(string const& spriteName, ActionType action, int numberOfActions, int numberOfFrames, int startFrame);
	static void setActionSpriteFrames(string const& spriteName, ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	static bool getIsStored(string const& spriteName);
	static void setIsStored(string const& spriteName);
	static ActionType actionStringToEnum(string const& action);
	static HandlingType handlingStringToEnum(string const& handling);
public:
	SpriteHolder();
	static void initSprites(string const& handlingType, string const& spriteName);
	static void setSprite(Sprite& sprite, string const& spriteName, string const& actionCategory, int actionType, int actionFrame);
	static int getMaxFramesForAction(string const& spriteName, string const& actionCategory, int actionType);
	static int getStartFramesForAction(string const& spriteName, string const& actionCategory, int actionType);
};

#endif 