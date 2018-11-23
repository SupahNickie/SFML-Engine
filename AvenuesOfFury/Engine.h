#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "SpriteHolder.h"
#include "Link.h"
#include "Skate.h"

using namespace sf;

class Engine {
private:
	TextureHolder th;
	SpriteHolder sh;
	RenderWindow window;
	View mainView;
	Link player1;
	Skate player2;

	void input();
	void update(float timeElapsed);
	void draw();
public:
	Engine();
	void run();
};