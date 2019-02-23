#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace sf;
using namespace std;

class Graphic : public Drawable {
public:
	string spriteName;
	unordered_map<string, bool> animationCycle; // Values only need to exist and be true if animation needs to cycle
	string texturePath;
	Sprite sprite;
	Vector2f position;

	void flipHorizontally();
	void setAnimationCycle(unordered_map<string, bool> cycle);
	unordered_map<string, bool> getAnimationCycle();
	void setTexturePath(string const& newTexturePath);
	void setSprite(Sprite const& newSprite);
	Sprite* getSprite();
	void deleteSprite();
	void setPosition(Vector2f position);
	Vector2f getPosition();
	FloatRect getBounds();
	void setSpriteName(string const& newSpriteName);
	string getSpriteName();
	virtual void draw(RenderTarget& target, RenderStates states) const;
};