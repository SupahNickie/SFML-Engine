#pragma once
#ifndef SPRITE_HOLDER_H
#define SPRITE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class SpriteHolder {
private:
	map<string, Vector2i**> moveSpriteOriginsMap;
	map<string, Vector2i**> moveSpriteBoundsMap;
	map<string, Vector2i**> attackSpriteOriginsMap;
	map<string, Vector2i**> attackSpriteBoundsMap;
	map<string, int*> moveTypeMaxFramesMap;
	map<string, int*> attackTypeMaxFramesMap;
	map<string, bool> isStoredMap;
	static SpriteHolder* shInstance;
public:
	SpriteHolder();
	static bool isStored(string const& charName);

	static Vector2i** getMoveSpriteOrigins(string const& charName);
	static void setMoveSpriteOrigins(string const& charName, Vector2i** sprites);
	
	static Vector2i** getMoveSpriteBounds(string const& charName);
	static void setMoveSpriteBounds(string const& charName, Vector2i** sprites);
	
	static Vector2i** getAttackSpriteOrigins(string const& charName);
	static void setAttackSpriteOrigins(string const& charName, Vector2i** sprites);
	
	static Vector2i** getAttackSpriteBounds(string const& charName);
	static void setAttackSpriteBounds(string const& charName, Vector2i** sprites);

	static int* getMoveTypeMaxFrames(string const& charName);
	static void setMoveTypeMaxFrames(string const& charName, int* counts);

	static int* getAttackTypeMaxFrames(string const& charName);
	static void setAttackTypeMaxFrames(string const& charName, int* counts);
};

#endif 