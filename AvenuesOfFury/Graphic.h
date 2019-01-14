#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class Graphic : public Drawable {
public:
	bool isActive;

	void flipHorizontally();
	void setPosition(Vector2f position);
	FloatRect getPosition();
	Vector2f getCenter();
	void deleteSprite();
	virtual void draw(RenderTarget& target, RenderStates states) const;
protected:
	string spriteName;
	map<string, bool> animationCycle; // Values only need to exist and be true if animation needs to cycle
	string texturePath;
	Sprite sprite;
	Vector2f position;
};