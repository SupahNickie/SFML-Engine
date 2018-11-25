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
	map<string, Vector2i**> idleSpriteOriginsMap;
	map<string, Vector2i**> idleSpriteBoundsMap;
	map<string, int*> moveTypeMaxFramesMap;
	map<string, int*> attackTypeMaxFramesMap;
	map<string, int*> idleTypeMaxFramesMap;
	map<string, int*> moveTypeStartFramesMap;
	map<string, int*> attackTypeStartFramesMap;
	map<string, int*> idleTypeStartFramesMap;
	map<string, bool> isStoredMap;
	static SpriteHolder* shInstance;
public:
	SpriteHolder();
	static bool getIsStored(string const& charName);
	static void setIsStored(string const& charName);

	static Vector2i** getMoveSpriteOrigins(string const& charName);
	static void setMoveSpriteOrigins(string const& charName, Vector2i** sprites);
	
	static Vector2i** getMoveSpriteBounds(string const& charName);
	static void setMoveSpriteBounds(string const& charName, Vector2i** sprites);
	
	static Vector2i** getAttackSpriteOrigins(string const& charName);
	static void setAttackSpriteOrigins(string const& charName, Vector2i** sprites);
	
	static Vector2i** getAttackSpriteBounds(string const& charName);
	static void setAttackSpriteBounds(string const& charName, Vector2i** sprites);

	static Vector2i** getIdleSpriteOrigins(string const& charName);
	static void setIdleSpriteOrigins(string const& charName, Vector2i** sprites);

	static Vector2i** getIdleSpriteBounds(string const& charName);
	static void setIdleSpriteBounds(string const& charName, Vector2i** sprites);

	static int* getMoveTypeMaxFrames(string const& charName);
	static void setMoveTypeMaxFrames(string const& charName, int* counts);

	static int* getAttackTypeMaxFrames(string const& charName);
	static void setAttackTypeMaxFrames(string const& charName, int* counts);

	static int* getIdleTypeMaxFrames(string const& charName);
	static void setIdleTypeMaxFrames(string const& charName, int* counts);

	static int* getMoveTypeStartFrames(string const& charName);
	static void setMoveTypeStartFrames(string const& charName, int* counts);

	static int* getAttackTypeStartFrames(string const& charName);
	static void setAttackTypeStartFrames(string const& charName, int* counts);

	static int* getIdleTypeStartFrames(string const& charName);
	static void setIdleTypeStartFrames(string const& charName, int* counts);
};

#endif 