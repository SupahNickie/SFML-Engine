#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "TextureHolder.h"
#include "SpriteHolder.h"
#include "Link.h"
#include "Skate.h"
#include "Garnet.h"

using namespace sf;

class Engine {
private:
	Globals g;
	TextureHolder th;
	SpriteHolder sh;
	RenderWindow window;
	View mainView;
	
	PlayerCharacter** players;
	EnemyCharacter** enemies;
	int numPlayers = 0;
	int numEnemies = 0;

	void input();
	void update(float timeElapsed);
	void draw();
public:
	Engine();
	void run();
};