#pragma once
#include "Globals.h"
#include "Graphic.h"

using namespace sf;
using namespace std;

struct CharacterVelocity {
	Vector2f position;
	Graphic::DirectionHeaded direction;
};

class Character : public Graphic {
protected:
	int MS_PER_FRAME = 50;
	int STUN_LENGTH = 200;
	int const MOVE_1 = 0;
	int const MOVE_2 = 1;
	int const ATTACK_1 = 0;
	int const ATTACK_2 = 1;
	int const IDLE_1 = 0;
	int const INJURE_1 = 0;

	map<int, CharacterVelocity> pastPositions;
	int timeSincePastPositionsUpdate = 0;
	Globals::ActionType spriteState;
	bool facingLeft;
	bool facingRight;
	float baseSpeed;
	bool jumping = false;
	bool running = false;
	bool disabled = false;

	int health = 0;
	vector<int> attackPower;
	bool attackDisabled = false;
	bool hitRegistered = false;

	vector<Character*> playersTouching;
	vector<Character*> enemiesTouching;

	bool currentActionDone = false;
	int currentFrame = 0;
	string currentAction;
	int currentActionType = 0;
	int timeSinceLastFrame = 0;
	int timeSinceLastAction = 0;
	bool spriteCycleDown = false;

	void detectCollisions(vector<Character*> players, vector<Character*> enemies);
	void resetFrameState();
	void updateFrameState(float elapsedTime);
	void updatePastPositions(float elapsedTime);
	void render();
	virtual void setDirectionHeaded() = 0;
public:
	Graphic::DirectionHeaded directionHeaded = Graphic::DirectionHeaded::NONE;
	float speed;
	unsigned int uniqueID;
	Character* focusChar;

	Character();
	void flipHorizontally();
	void disable();
	Graphic::DirectionHeaded stringToDirection(string const& direction);
	bool hits(Character* otherChar);
	void registerHit(int hp);
	CharacterVelocity getVelocity(int time);
	virtual void update(float timeElapsed, vector<Character*> players, vector<Character*> enemies) = 0;
};