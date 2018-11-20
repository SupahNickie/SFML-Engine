#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class PlayerCharacter : public Drawable {
protected:
	int const MS_PER_FRAME = 50;
	enum class SpriteState { IDLE, ATTACKING };

	SpriteState spriteState;
	Sprite sprite;
	Vector2f position;
	
	Vector2i* attack1SpriteOrigins = nullptr;
	Vector2i* attack1SpriteBounds = nullptr;
	Vector2i* move1SpriteOrigins = nullptr;
	Vector2i* move1SpriteBounds = nullptr;

	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool attackPressed;
	bool attackDisabled = false;
	
	bool facingLeft;
	bool facingRight;
	bool jumping;
	
	int idle1Frame = 0;
	int attack1Frame = 0;
	int move1Frame = 0;
	int idle1FrameMax;
	int attack1FrameMax;
	int move1FrameMax;
	int timeSinceIdle1Frame = 0;
	int timeSinceAttack1Frame = 0;
	int timeSinceMove1Frame = 0;
	bool moveSpriteCycleDown;

	float speed;

	void virtual initAttackSprites() = 0;
	void virtual setIdleSprite() = 0;
	void virtual handleAttack(float elapsedTime) = 0;
	void virtual handleMove(float elapsedTime) = 0;
	void virtual setAttack1Sprite() = 0;
	void virtual setMove1Sprite() = 0;
private:
	Texture texture;
	FloatRect hitbox;

	void flipHorizontally();
public:
	virtual void draw(RenderTarget& target, RenderStates states) const;
	void update(float elapsedTime);
};