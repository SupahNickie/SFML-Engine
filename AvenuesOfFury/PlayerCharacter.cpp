#include "pch.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "PlayerCharacter.h"
#include "SpriteHolder.h"

using namespace sf;
using namespace std;

void PlayerCharacter::update(float elapsedTime) {
	if (primaryAttackPressed) {
		handleAttack(elapsedTime, ATTACK_1);
	}
	else if (secondaryAttackPressed) {
		handleAttack(elapsedTime, ATTACK_2);
	}
	else if (spriteState == SpriteState::ATTACKING && !primaryAttackPressed && !secondaryAttackPressed) {
		resetIdleFrame(IDLE_1);
		spriteState = SpriteState::IDLE;
		handleIdle(elapsedTime, IDLE_1);
		resetAttackFrame(ATTACK_1);
		timeSinceAttackFrame = 0;
		attackDisabled = false;
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		handleIdle(elapsedTime, IDLE_1);
	}

	if (rightPressed) {
		position.x += speed * elapsedTime;
		if (!facingRight) flipHorizontally();
		handleMove(elapsedTime, MOVE_1);
	}

	if (leftPressed) {
		position.x -= speed * elapsedTime;
		if (!facingLeft) flipHorizontally();
		handleMove(elapsedTime, MOVE_1);
	}

	if (upPressed) {
		position.y -= speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime, MOVE_1);
	}

	if (downPressed) {
		position.y += speed * elapsedTime;
		if (!rightPressed && !leftPressed) handleMove(elapsedTime, MOVE_1);
	}

	sprite.setPosition(position);
}

void PlayerCharacter::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite, states);
}

void PlayerCharacter::flipHorizontally() {
	sprite.scale(-1, 1);
	facingLeft = !facingLeft;
	facingRight = !facingRight;
}

void PlayerCharacter::handleMove(float elapsedTime, int moveType) {
	timeSinceMoveFrame += elapsedTime * 1000;
	if (timeSinceMoveFrame > MS_PER_FRAME) {
		moveSpriteCycleDown ? --moveFrame : ++moveFrame;
		timeSinceMoveFrame = 0;
	}

	if (moveFrame >= SpriteHolder::getMoveTypeMaxFrames(charName)[moveType] && !moveSpriteCycleDown) {
		moveSpriteCycleDown = true;
		--moveFrame;
		--moveFrame;
	}
	else if (moveFrame < 0 && moveSpriteCycleDown) {
		moveSpriteCycleDown = false;
		++moveFrame;
		++moveFrame;
	}

	setMoveSprite(moveType);
}

void PlayerCharacter::handleAttack(float elapsedTime, int attackType) {
	if (attackDisabled) return;
	timeSinceAttackFrame += elapsedTime * 1000;
	if (timeSinceAttackFrame > MS_PER_FRAME) {
		++attackFrame;
		timeSinceAttackFrame = 0;
	}

	if (attackFrame >= SpriteHolder::getAttackTypeMaxFrames(charName)[attackType]) {
		setIdleSprite(IDLE_1);
		spriteState = SpriteState::IDLE;
		resetAttackFrame(SpriteHolder::getAttackTypeStartFrames(charName)[attackType]);
		idleFrame = 0;
		timeSinceAttackFrame = 0;
		primaryAttackPressed = false;
		secondaryAttackPressed = false;
		attackDisabled = true;
	}
	else {
		setAttackSprite(attackType);
	}
}

void PlayerCharacter::handleIdle(float elapsedTime, int idleType) {
	timeSinceIdleFrame += elapsedTime * 1000;
	if (timeSinceIdleFrame > MS_PER_FRAME) {
		idleSpriteCycleDown ? --idleFrame : ++idleFrame;
		timeSinceIdleFrame = 0;
	}

	if (idleFrame >= SpriteHolder::getIdleTypeMaxFrames(charName)[idleType] && !idleSpriteCycleDown) {
		idleSpriteCycleDown = true;
		--idleFrame;
		--idleFrame;
	}
	else if (idleFrame < 0 && idleSpriteCycleDown) {
		idleSpriteCycleDown = false;
		++idleFrame;
		++idleFrame;
	}

	setIdleSprite(idleType);
}

void PlayerCharacter::initSprites() {
	if (SpriteHolder::getIsStored(charName)) return;

	string currentlySetting = "";
	unsigned int numberOfActions = 0;
	int outIndex = 0;
	int inIndex = 0;
	unsigned int numberOfFrames = 0;
	unsigned int startFrame = 0;

	stringstream ss;
	ss << "graphics/" << charName << "_coords.txt";
	
	string s;
	const string delimiter = ",";
	ifstream inputFile(ss.str());
	while (getline(inputFile, s)) {
		size_t pos = 0;
		string current = "";
		vector<string> line{};

		while ((pos = s.find(delimiter)) != string::npos) {
			current = s.substr(0, pos);
			line.push_back(current);
			s.erase(0, pos + delimiter.length());
		}
		line.push_back(s);

		if (line[0] == "move" || line[0] == "attack" || line[0] == "idle") {
			currentlySetting = line[0];
			numberOfActions = stoi(line[1]);
			initActionSprites(actionStringToEnum(currentlySetting), numberOfActions);
		}
		else if (line[0] == "frames") {
			inIndex = 0;
			outIndex = 0;
			numberOfActions = 0;
			for (unsigned int i = 1; i < line.size(); ++numberOfActions) {
				numberOfFrames = stoi(line[i]);
				++i;
				startFrame = stoi(line[i]);
				++i;
				addActionSpriteFrames(actionStringToEnum(currentlySetting), numberOfActions, numberOfFrames, startFrame);
			}
		}
		else if (line[0] == "skip") {
			inIndex = 0;
			++outIndex;
		}
		else {
			setActionSpriteFrames(
				actionStringToEnum(currentlySetting),
				outIndex,
				inIndex,
				Vector2i(stoi(line[0]), stoi(line[1])),
				Vector2i(stoi(line[2]), stoi(line[3]))
			);
			++inIndex;
		}
	}
	inputFile.close();

	resetMoveFrame(MOVE_1);
	resetAttackFrame(ATTACK_1);
	resetIdleFrame(IDLE_1);

	SpriteHolder::setIsStored(charName);
}

PlayerCharacter::ActionType PlayerCharacter::actionStringToEnum(string action) {
	if (action == "move") return PlayerCharacter::ActionType::MOVE;
	if (action == "attack") return PlayerCharacter::ActionType::ATTACK;
	if (action == "idle") return PlayerCharacter::ActionType::IDLE;
}

void PlayerCharacter::initActionSprites(PlayerCharacter::ActionType action, unsigned int actionCount) {
	switch (action) {
	case PlayerCharacter::ActionType::MOVE: {
		Vector2i** moveSpriteOrigins = new Vector2i*[actionCount];
		SpriteHolder::setMoveSpriteOrigins(charName, moveSpriteOrigins);

		Vector2i** moveSpriteBounds = new Vector2i*[actionCount];
		SpriteHolder::setMoveSpriteBounds(charName, moveSpriteBounds);

		int* moveTypeMaxFrames = new int[actionCount];
		SpriteHolder::setMoveTypeMaxFrames(charName, moveTypeMaxFrames);

		int* moveTypeStartFrames = new int[actionCount];
		SpriteHolder::setMoveTypeStartFrames(charName, moveTypeStartFrames);
	}
		break;
	case PlayerCharacter::ActionType::ATTACK: {
		Vector2i** attackSpriteOrigins = new Vector2i*[actionCount];
		SpriteHolder::setAttackSpriteOrigins(charName, attackSpriteOrigins);

		Vector2i** attackSpriteBounds = new Vector2i*[actionCount];
		SpriteHolder::setAttackSpriteBounds(charName, attackSpriteBounds);

		int* attackTypeMaxFrames = new int[actionCount];
		SpriteHolder::setAttackTypeMaxFrames(charName, attackTypeMaxFrames);

		int* attackTypeStartFrames = new int[actionCount];
		SpriteHolder::setAttackTypeStartFrames(charName, attackTypeStartFrames);
	}
		break;
	case PlayerCharacter::ActionType::IDLE: {
		Vector2i** idleSpriteOrigins = new Vector2i*[actionCount];
		SpriteHolder::setIdleSpriteOrigins(charName, idleSpriteOrigins);

		Vector2i** idleSpriteBounds = new Vector2i*[actionCount];
		SpriteHolder::setIdleSpriteBounds(charName, idleSpriteBounds);

		int* idleTypeMaxFrames = new int[actionCount];
		SpriteHolder::setIdleTypeMaxFrames(charName, idleTypeMaxFrames);

		int* idleTypeStartFrames = new int[actionCount];
		SpriteHolder::setIdleTypeStartFrames(charName, idleTypeStartFrames);
	}
		break;
	}
}

void PlayerCharacter::addActionSpriteFrames(PlayerCharacter::ActionType action, int numberOfActions, int numberOfFrames, int startFrame) {
	switch (action) {
	case PlayerCharacter::ActionType::MOVE:
		SpriteHolder::getMoveSpriteOrigins(charName)[numberOfActions] = new Vector2i[numberOfFrames];
		SpriteHolder::getMoveSpriteBounds(charName)[numberOfActions] = new Vector2i[numberOfFrames];
		SpriteHolder::getMoveTypeMaxFrames(charName)[numberOfActions] = numberOfFrames;
		SpriteHolder::getMoveTypeStartFrames(charName)[numberOfActions] = startFrame;
		break;
	case PlayerCharacter::ActionType::ATTACK:
		SpriteHolder::getAttackSpriteOrigins(charName)[numberOfActions] = new Vector2i[numberOfFrames];
		SpriteHolder::getAttackSpriteBounds(charName)[numberOfActions] = new Vector2i[numberOfFrames];
		SpriteHolder::getAttackTypeMaxFrames(charName)[numberOfActions] = numberOfFrames;
		SpriteHolder::getAttackTypeStartFrames(charName)[numberOfActions] = startFrame;
		break;
	case PlayerCharacter::ActionType::IDLE:
		SpriteHolder::getIdleSpriteOrigins(charName)[numberOfActions] = new Vector2i[numberOfFrames];
		SpriteHolder::getIdleSpriteBounds(charName)[numberOfActions] = new Vector2i[numberOfFrames];
		SpriteHolder::getIdleTypeMaxFrames(charName)[numberOfActions] = numberOfFrames;
		SpriteHolder::getIdleTypeStartFrames(charName)[numberOfActions] = startFrame;
		break;
	}
}

void PlayerCharacter::setActionSpriteFrames(PlayerCharacter::ActionType action, int outIndex, int inIndex, Vector2i origin, Vector2i bound) {
	switch (action) {
	case PlayerCharacter::ActionType::MOVE:
		SpriteHolder::getMoveSpriteOrigins(charName)[outIndex][inIndex] = origin;
		SpriteHolder::getMoveSpriteBounds(charName)[outIndex][inIndex] = bound;
		break;
	case PlayerCharacter::ActionType::ATTACK:
		SpriteHolder::getAttackSpriteOrigins(charName)[outIndex][inIndex] = origin;
		SpriteHolder::getAttackSpriteBounds(charName)[outIndex][inIndex] = bound;
		break;
	case PlayerCharacter::ActionType::IDLE:
		SpriteHolder::getIdleSpriteOrigins(charName)[outIndex][inIndex] = origin;
		SpriteHolder::getIdleSpriteBounds(charName)[outIndex][inIndex] = bound;
		break;
	}
}

void PlayerCharacter::setMoveSprite(int moveType) {
	if (spriteState == SpriteState::IDLE) {
		spriteState = SpriteState::MOVING;
		resetMoveFrame(moveType);
	}

	sprite.setTextureRect(
		IntRect(
			SpriteHolder::getMoveSpriteOrigins(charName)[moveType][moveFrame].x,
			SpriteHolder::getMoveSpriteOrigins(charName)[moveType][moveFrame].y,
			SpriteHolder::getMoveSpriteBounds(charName)[moveType][moveFrame].x,
			SpriteHolder::getMoveSpriteBounds(charName)[moveType][moveFrame].y
		)
	);
	sprite.setOrigin(
		SpriteHolder::getMoveSpriteBounds(charName)[moveType][moveFrame].x / 2.0f,
		SpriteHolder::getMoveSpriteBounds(charName)[moveType][moveFrame].y / 2.0f
	);
}

void PlayerCharacter::setAttackSprite(int attackType) {
	sprite.setTextureRect(
		IntRect(
			SpriteHolder::getAttackSpriteOrigins(charName)[attackType][attackFrame].x,
			SpriteHolder::getAttackSpriteOrigins(charName)[attackType][attackFrame].y,
			SpriteHolder::getAttackSpriteBounds(charName)[attackType][attackFrame].x,
			SpriteHolder::getAttackSpriteBounds(charName)[attackType][attackFrame].y
		)
	);
	sprite.setOrigin(
		SpriteHolder::getAttackSpriteBounds(charName)[attackType][attackFrame].x / 2.0f,
		SpriteHolder::getAttackSpriteBounds(charName)[attackType][attackFrame].y / 2.0f
	);
}

void PlayerCharacter::setIdleSprite(int idleType) {
	sprite.setTextureRect(
		IntRect(
			SpriteHolder::getIdleSpriteOrigins(charName)[idleType][idleFrame].x,
			SpriteHolder::getIdleSpriteOrigins(charName)[idleType][idleFrame].y,
			SpriteHolder::getIdleSpriteBounds(charName)[idleType][idleFrame].x,
			SpriteHolder::getIdleSpriteBounds(charName)[idleType][idleFrame].y
		)
	);
	sprite.setOrigin(
		SpriteHolder::getIdleSpriteBounds(charName)[idleType][idleFrame].x / 2.0f,
		SpriteHolder::getIdleSpriteBounds(charName)[idleType][idleFrame].y / 2.0f
	);
}

void PlayerCharacter::resetMoveFrame(int moveType) {
	moveFrame = SpriteHolder::getMoveTypeStartFrames(charName)[moveType];
}

void PlayerCharacter::resetAttackFrame(int attackType) {
	attackFrame = SpriteHolder::getAttackTypeStartFrames(charName)[attackType];
}

void PlayerCharacter::resetIdleFrame(int idleType) {
	idleFrame = SpriteHolder::getIdleTypeStartFrames(charName)[idleType];
}