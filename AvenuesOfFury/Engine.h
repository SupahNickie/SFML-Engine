#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Link.h"

using namespace sf;

class Engine {
private:
	TextureHolder th;
	RenderWindow window;
	View mainView;
	Link player1;

	void input();
	void update(float timeElapsed);
	void draw();
public:
	Engine();
	void run();
};