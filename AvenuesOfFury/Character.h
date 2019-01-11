#pragma once
#include "Globals.h"
#include "Graphic.h"

using namespace sf;
using namespace std;

struct CharacterVelocity {
	Vector2f position;
	Graphic::DirectionHeaded direction;
};

struct HitRecord {
	unsigned short int frame;
	unsigned int timeSinceHitRegistered;
};

struct AttackInfo {
	unsigned short int injuryType; // INJURE_HEAD, INJURE_BODY
	string action; // fall, injure
	Globals::ActionType actionType;
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
	int const INJURE_HEAD = 0;
	int const INJURE_BODY = 1;
	int const JUMP_START = 0;
	int const JUMP_AIR = 0;
	int const JUMP_LAND = 0;
	int const JUMP_ATTACK = 0;
	int const RUN_ATTACK = 0;
	int const RUN_ATTACK_LAND = 0;

	map<int, CharacterVelocity> pastPositions;
	Graphic::DirectionHeaded* pastDirectionsPressed;
	int timeSincePastPositionsUpdate = 0;
	Globals::ActionType spriteState;
	bool facingRight;
	float baseSpeed;
	bool running = false;
	bool disabled = false;

	int health = 0;
	vector<int> attackPower;
	bool attackDisabled = false;

	map<string, HitRecord> hitsRegistered;
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

	bool jumping = false;
	bool jumpDisabled = false;
	float prejumpY = 0.0f;
	int jumpLength = 0;

	void advanceHitRecords(float elapsedTime);
	void detectCollisions(vector<Character*> players, vector<Character*> enemies);
	void resetFrameState(bool clearAll = true);
	void updateFrameState(float elapsedTime, bool prioritizedAction, bool jumping);
	void updatePastPositions(float elapsedTime);
	void insertAndShiftPastDirectionsPressed(Graphic::DirectionHeaded direction);
	void setAttackState(int attackType);
	AttackInfo generateAttackInfo();
	void render();
	virtual void setIdleState(float elapsedTime = 0.0f) = 0;
	virtual void setDirectionHeaded() = 0;
private:
	bool handleJumpingAnimation(int maxFrames, bool attacking);
	void handleNormalAnimation(int maxFrames);
public:
	Graphic::DirectionHeaded directionHeaded = Graphic::DirectionHeaded::NONE;
	float speed;
	unsigned int uniqueID;
	Character* focusChar;

	Character();
	void flipHorizontally();
	void disable();
	Graphic::DirectionHeaded stringToDirection(string const& direction);
	string directionToString(Graphic::DirectionHeaded direction);
	bool hits(Character* otherChar);
	void registerHit(int hp, string const& attacker, unsigned short int frame, AttackInfo info);
	CharacterVelocity getVelocity(int time);
	virtual void update(float timeElapsed, vector<Character*> players, vector<Character*> enemies) = 0;
};