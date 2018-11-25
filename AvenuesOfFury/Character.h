#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Character : public Drawable {
protected:
	int MS_PER_FRAME = 50;
	int const MOVE_1 = 0;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	int const IDLE_1 = 0;
	float speed;

	enum class SpriteState { MOVING, ATTACKING, IDLE };
	enum class ActionType { MOVE, ATTACK, IDLE };

	string charName;
	SpriteState spriteState;
	Sprite sprite;
	Vector2f position;

	bool animationCycle;
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
	bool attackSpriteCycleDown;
	bool idleSpriteCycleDown;
	int timeSinceHandlingLastAttackFrame = 0;

	void flipHorizontally();
	
	void renderMove(float elapsedTime, int moveType);
	void renderAttack(float elapsedTime, int attackType);
	void renderIdle(float elapsedTime, int idleType);
	
	void initSprites();

	void resetMoveFrame(int moveType);
	void resetAttackFrame(int attackType);
	void resetIdleFrame(int idleType);
	
	virtual void handleMove(float elapsedTime, int moveType) = 0;
	virtual void handleAttack(float elapsedTime, int attackType) = 0;
	virtual void handleIdle(float elapsedTime, int idleType) = 0;
	
	virtual void draw(RenderTarget& target, RenderStates states) const;
private:
	Texture texture;
	FloatRect hitbox;

	ActionType actionStringToEnum(string action);
	void initActionSprites(ActionType action, unsigned int moveCount);
	void addActionSpriteFrames(ActionType action, int numberOfActions, int numberOfFrames, int startFrame);
	void setActionSpriteFrames(ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound);
	
	void setMoveSprite(int moveType);
	void setAttackSprite(int attackType);
	void setIdleSprite(int idleType);
public:
	void setPosition(Vector2f position);

	void virtual handleInput() = 0;
	void virtual update(float elapsedTime) = 0;
};