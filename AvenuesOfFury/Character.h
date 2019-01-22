#pragma once
#include "Globals.h"
#include "Graphic.h"

using namespace sf;
using namespace std;

struct AttackInfo;
struct CharacterVelocity;
struct HitRecord;
class Character : public Graphic {
public:
	enum class DirectionHeaded { U, UR, R, DR, D, DL, L, UL, NONE };
	enum class FallStep { START_FALL, KNOCK_DOWN, BOUNCE_UP, NONE };
	enum class FallDirection { LEFT, RIGHT, NONE };
	DirectionHeaded directionHeaded = DirectionHeaded::NONE;
	float speed;
	unsigned int uniqueID;
	Character* focusChar;

	Character();
	void flipHorizontally();
	void disable(int timeToDisable);
	DirectionHeaded stringToDirection(string const& direction);
	string directionToString(DirectionHeaded direction);
	bool hits(Character* otherChar);
	void registerHit(int hp, string const& attacker, unsigned short int frame, AttackInfo info);
	CharacterVelocity getVelocity(int time);
	virtual void update(float timeElapsed, vector<Character*> players, vector<Character*> enemies) = 0;
protected:
	int MS_PER_FRAME = 50;
	int STUN_LENGTH = 200;
	int const WALK = 0;
	int const RUN = 1;
	int const HEAD_ATTACK = 0;
	int const BODY_ATTACK = 1;
	int const NORMAL_IDLE = 0;
	int const INJURE_HEAD = 0;
	int const INJURE_BODY = 1;
	int const JUMP_START = 0;
	int const JUMP_AIR = 0;
	int const JUMP_LAND = 0;
	int const JUMP_ATTACK = 0;
	int const RUN_ATTACK = 0;
	int const RUN_ATTACK_LAND = 0;
	int const FALL = 0;
	int const RISE = 0;

	bool grabbing = false;

	float baseSpeed;
	bool running = false;
	bool runAttacking = false;
	bool runAttackJumps = false;

	bool jumping = false;
	bool jumpDisabled = false;
	float prejumpY = 0.0f;

	int timeToBeDisabled = 0;
	bool disabled = false;
	float fallY = 0.0f;
	float speedY = 0.0f;
	float baseSpeedY = 0.0f; // Adjust as per needed to get jump frame timing right
	float const gravity = 0.07f * Globals::getScalingFactor();
	FallStep fallstatus = FallStep::NONE;
	FallDirection fallDirection = FallDirection::NONE;

	int health = 0;
	vector<int> attackPower;
	bool attackDisabled = false;

	map<string, HitRecord> hitsRegistered;
	bool hitRegistered = false;
	bool zig = false; // or zag, for "bouncing" when hit

	vector<Character*> playersTouching;
	vector<Character*> enemiesTouching;

	bool currentActionDone = false;
	int currentFrame = 0;
	string currentAction;
	int currentActionType = 0;
	int timeSinceLastFrame = 0;
	int timeSinceLastAction = 0;
	bool spriteCycleDown = false;

	map<int, CharacterVelocity> pastPositions;
	DirectionHeaded* pastDirectionsPressed;
	int timeSincePastPositionsUpdate = 0;
	Globals::ActionType spriteState;
	bool facingRight;

	void advanceHitRecords(float elapsedTime);
	void detectCollisions(vector<Character*> players, vector<Character*> enemies);
	bool onSameVerticalPlane(float targetY);
	void resetFrameState(bool clearAll = true);
	void updateFrameState(float elapsedTime, string info);
	void updatePastPositions(float elapsedTime);
	void insertAndShiftPastDirectionsPressed(DirectionHeaded direction);
	void setAttackState(string const& action, int attackType, bool resetFrame = true);
	void setJumpState(float elapsedTime, bool moveLeft, bool moveRight);
	AttackInfo generateAttackInfo(bool longStun, Character* c);
	void render();
	virtual void setIdleState(float elapsedTime) = 0;
	virtual void setDirectionHeaded() = 0;
	virtual void handleRunAttackHorizontal(float elapsedTime) = 0;
private:
	bool handleGrabbingAnimation(int maxFrames, string info, float elapsedTime);
	bool handleRunningAnimation(int maxFrames, string info, float elapsedTime);
	bool handleJumpingAnimation(int maxFrames, string info, float elapsedTime);
	bool handleFallingAnimation(int maxFrames, string info, float elapsedTime);
	bool handleInjureAnimation(int maxFrames, string info, float elapsedTime);
	void handleNormalAnimation(int maxFrames);
	FallDirection getDirectionOfCollision(Character* c);
};

struct CharacterVelocity {
	Vector2f position;
	Character::DirectionHeaded direction;
};

struct HitRecord {
	unsigned short int frame;
	unsigned int timeSinceHitRegistered;
};

struct AttackInfo {
	string buttonPress;
	unsigned short int injuryType; // INJURE_HEAD, INJURE_BODY
	string action; // fall, injure
	Globals::ActionType actionType;
	unsigned int timeToDisable;
	Character::FallStep fallstatus;
	float fallY;
	Character::FallDirection fallDirection;
};