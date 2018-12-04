#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Graphic : public Drawable {
protected:
	string spriteName;
	bool animationCycle;

	Sprite sprite;
	Vector2f position;
private:
	Texture texture;
public:
	bool isActive;
	void flipHorizontally();
	void setPosition(Vector2f position);
	void deleteSprite();

	virtual void draw(RenderTarget& target, RenderStates states) const;
};