#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Area {
private:
	Sprite sprite;
	Vector2f viewPosition;

public:
	Area(string const& spriteFilename);
	Vector2f getViewPosition();
};