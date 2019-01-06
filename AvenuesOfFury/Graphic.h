#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class Graphic : public Drawable {
protected:
	string spriteName;
	map<string, bool> animationCycle;
	string texturePath;
	Sprite sprite;
	Vector2f position;
public:
	enum class DirectionHeaded { U, UR, R, DR, D, DL, L, UL, NONE };

	bool isActive;
	void flipHorizontally();
	void setPosition(Vector2f position);
	FloatRect getPosition();
	Vector2f getCenter();
	void deleteSprite();

	virtual void draw(RenderTarget& target, RenderStates states) const;
};