#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class PlayerCharacter : public Drawable {
protected:
	int const MS_PER_FRAME = 50;
	int const MOVEMENT_TYPES = 1;
	int const MOVE_1 = 0;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	enum class SpriteState { IDLE, ATTACKING, MOVING };

	std::string charName;
	SpriteState spriteState;
	Sprite sprite;
	Vector2f position;
	
	Vector2i** moveSpriteOrigins = nullptr;
	Vector2i** moveSpriteBounds = nullptr;
	Vector2i** attackSpriteOrigins = nullptr;
	Vector2i** attackSpriteBounds = nullptr;
	int* moveTypeMaxFrames = nullptr;
	int* attackTypeMaxFrames = nullptr;

	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool primaryAttackPressed;
	bool secondaryAttackPressed;
	bool attackDisabled = false;
	
	bool facingLeft;
	bool facingRight;
	bool jumping;
	
	int idleFrame = 0;
	int attackFrame = 0;
	int moveFrame = 0;
	int idleFrameMax;
	int timeSinceIdleFrame = 0;
	int timeSinceAttackFrame = 0;
	int timeSinceMoveFrame = 0;
	bool moveSpriteCycleDown;

	float speed;

	void initSprites();
	void virtual setIdleSprite() = 0;
	void handleAttack(float elapsedTime, int attackType);
	void handleMove(float elapsedTime, int moveType);
	void setAttackSprite(int attackType);
	void setMoveSprite(int moveType);
	void resetAttackFrame();
	void virtual resetMoveFrame(int moveType) = 0;
private:
	Texture texture;
	FloatRect hitbox;

	void initMoveSprites(unsigned int moveCount);
	void initAttackSprites(unsigned int moveCount);
	void addMoveSpriteFrames(unsigned int numberOfFrames, int index);
	void addAttackSpriteFrames(unsigned int numberOfFrames, int index);
	void setMoveSpriteFrames(int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	void setAttackSpriteFrames(int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	void flipHorizontally();
public:
	virtual void draw(RenderTarget& target, RenderStates states) const;
	void update(float elapsedTime);
};