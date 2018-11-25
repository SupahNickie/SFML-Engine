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
	enum class ActionType { MOVE, ATTACK, IDLE };

	map<string, map<ActionType, Vector2i**>> charOriginsMap;
	map<string, map<ActionType, Vector2i**>> charBoundsMap;
	map<string, map<ActionType, int*>> charMaxFrameMap;
	map<string, map<ActionType, int*>> charStartFrameMap;
	map<string, bool> isStoredMap;

	static void initActionSprites(string const& charName, ActionType action, unsigned int moveCount);
	static void addActionSpriteFrames(string const& charName, ActionType action, int numberOfActions, int numberOfFrames, int startFrame);
	static void setActionSpriteFrames(string const& charName, ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	static bool getIsStored(string const& charName);
	static void setIsStored(string const& charName);
	static ActionType actionStringToEnum(string const& action);
public:
	SpriteHolder();
	static void initSprites(string const& charName);
	static void setSprite(Sprite& sprite, string const& charName, string const& actionCategory, int actionType, int actionFrame);
	static int getMaxFramesForAction(string const& charName, string const& actionCategory, int actionType);
	static int getStartFramesForAction(string const& charName, string const& actionCategory, int actionType);
};

#endif 