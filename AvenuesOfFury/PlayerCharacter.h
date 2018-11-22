#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class PlayerCharacter : public Drawable {
protected:
	int const MS_PER_FRAME = 50;
	int const MOVE_1 = 0;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	int const IDLE_1 = 0;
	enum class SpriteState { MOVING, ATTACKING, IDLE };
	enum class ActionType { MOVE, ATTACK, IDLE };

	string charName;
	SpriteState spriteState;
	Sprite sprite;
	Vector2f position;

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

	int moveFrame = 0;
	int attackFrame = 0;
	int idleFrame = 0;
	int timeSinceMoveFrame = 0;
	int timeSinceAttackFrame = 0;
	int timeSinceIdleFrame = 0;
	bool moveSpriteCycleDown;

	float speed;

	void initSprites();

	void handleMove(float elapsedTime, int moveType);
	void handleAttack(float elapsedTime, int attackType);
	void handleIdle(float elapsedTime, int idleType);

	void setAttackSprite(int attackType);
	void setMoveSprite(int moveType);
	void setIdleSprite(int idleType);

	void resetMoveFrame(int moveType);
	void resetAttackFrame(int attackType);
	void resetIdleFrame(int idleType);
private:
	Texture texture;
	FloatRect hitbox;

	void initActionSprites(ActionType action, unsigned int moveCount);
	void addActionSpriteFrames(ActionType action, int numberOfActions, int numberOfFrames, int startFrame);
	void setActionSpriteFrames(ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	ActionType actionStringToEnum(string action);

	void flipHorizontally();
public:
	virtual void draw(RenderTarget& target, RenderStates states) const;
	void update(float elapsedTime);
};