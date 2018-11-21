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
		setIdleSprite();
		spriteState = SpriteState::IDLE;
		attackFrame = 0;
		idleFrame = 0;
		timeSinceAttackFrame = 0;
		attackDisabled = false;
	}
	else if (!upPressed && !downPressed && !leftPressed && !rightPressed) {
		setIdleSprite();
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

void PlayerCharacter::resetAttackFrame() {
	attackFrame = 0;
}

void PlayerCharacter::handleAttack(float elapsedTime, int attackType) {
	if (attackDisabled) return;
	timeSinceAttackFrame += elapsedTime * 1000;
	if (timeSinceAttackFrame > MS_PER_FRAME) {
		++attackFrame;
		timeSinceAttackFrame = 0;
	}

	if (attackFrame >= attackTypeMaxFrames[attackType]) {
		setIdleSprite();
		spriteState = SpriteState::IDLE;
		resetAttackFrame();
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

void PlayerCharacter::handleMove(float elapsedTime, int moveType) {
	timeSinceMoveFrame += elapsedTime * 1000;
	if (timeSinceMoveFrame > MS_PER_FRAME) {
		moveSpriteCycleDown ? --moveFrame : ++moveFrame;
		timeSinceMoveFrame = 0;
	}

	if (moveFrame >= moveTypeMaxFrames[moveType] && !moveSpriteCycleDown) {
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

void PlayerCharacter::initSprites() {
	if (SpriteHolder::isStored(charName)) {
		moveSpriteOrigins = SpriteHolder::getMoveSpriteOrigins(charName);
		moveSpriteBounds= SpriteHolder::getMoveSpriteBounds(charName);
		attackSpriteOrigins = SpriteHolder::getAttackSpriteOrigins(charName);
		attackSpriteBounds = SpriteHolder::getAttackSpriteBounds(charName);
		moveTypeMaxFrames = SpriteHolder::getMoveTypeMaxFrames(charName);
		attackTypeMaxFrames = SpriteHolder::getAttackTypeMaxFrames(charName);
		return;
	}

	bool settingMove = false;
	bool settingAttack = false;
	unsigned int numberOfMoves = 0;
	unsigned int numberOfAttacks = 0;
	int outIndex = 0;
	int inIndex = 0;
	unsigned int numberOfFrames = 0;

	stringstream ss;
	ss << "graphics/" << charName << "coords.txt";
	
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

		if (line[0] == "move") {
			settingMove = true;
			settingAttack = false;
			numberOfMoves = stoi(line[1]);
			initMoveSprites(numberOfMoves);
		}
		else if (line[0] == "attack") {
			settingAttack = true;
			settingMove = false;
			numberOfAttacks = stoi(line[1]);
			initAttackSprites(numberOfAttacks);
		}
		else if (line[0] == "frames" && settingMove) {
			inIndex = 0;
			outIndex = 0;
			for (unsigned int i = 1; i < line.size(); ++i) {
				numberOfFrames = stoi(line[i]);
				addMoveSpriteFrames(numberOfFrames, i - 1);
			}
		}
		else if (line[0] == "frames" && settingAttack) {
			outIndex = 0;
			inIndex = 0;
			for (unsigned int i = 1; i < line.size(); ++i) {
				numberOfFrames = stoi(line[i]);
				addAttackSpriteFrames(numberOfFrames, i - 1);
			}
		}
		else if (line[0] == "skip") {
			inIndex = 0;
			++outIndex;
		}
		else {
			if (settingMove) {
				setMoveSpriteFrames(
					outIndex, 
					inIndex, 
					Vector2i(stoi(line[0]), stoi(line[1])), 
					Vector2i(stoi(line[2]), stoi(line[3]))
				);
			}
			else if (settingAttack) {
				setAttackSpriteFrames(
					outIndex,
					inIndex,
					Vector2i(stoi(line[0]), stoi(line[1])),
					Vector2i(stoi(line[2]), stoi(line[3]))
				);
			}
			++inIndex;
		}
	}
	inputFile.close();
}

void PlayerCharacter::initMoveSprites(unsigned int moveCount) {
	moveSpriteOrigins = new Vector2i*[moveCount];
	SpriteHolder::setMoveSpriteOrigins(charName, moveSpriteOrigins);

	moveSpriteBounds = new Vector2i*[moveCount];
	SpriteHolder::setMoveSpriteBounds(charName, moveSpriteBounds);

	moveTypeMaxFrames = new int[moveCount];
	SpriteHolder::setMoveTypeMaxFrames(charName, moveTypeMaxFrames);

	resetMoveFrame(MOVE_1);
}

void PlayerCharacter::initAttackSprites(unsigned int moveCount) {
	attackSpriteOrigins = new Vector2i*[moveCount];
	SpriteHolder::setAttackSpriteOrigins(charName, attackSpriteOrigins);

	attackSpriteBounds = new Vector2i*[moveCount];
	SpriteHolder::setAttackSpriteBounds(charName, attackSpriteBounds);

	attackTypeMaxFrames = new int[moveCount];
	SpriteHolder::setAttackTypeMaxFrames(charName, attackTypeMaxFrames);

	resetAttackFrame();
}

void PlayerCharacter::addMoveSpriteFrames(unsigned int numberOfFrames, int index) {
	moveSpriteOrigins[index] = new Vector2i[numberOfFrames];
	moveSpriteBounds[index] = new Vector2i[numberOfFrames];
	moveTypeMaxFrames[index] = numberOfFrames;
}

void PlayerCharacter::addAttackSpriteFrames(unsigned int numberOfFrames, int index) {
	attackSpriteOrigins[index] = new Vector2i[numberOfFrames];
	attackSpriteBounds[index] = new Vector2i[numberOfFrames];
	attackTypeMaxFrames[index] = numberOfFrames;
}

void PlayerCharacter::setMoveSpriteFrames(int outIndex, int inIndex, Vector2i origin, Vector2i bound) {
	moveSpriteOrigins[outIndex][inIndex] = origin;
	moveSpriteBounds[outIndex][inIndex] = bound;
}

void PlayerCharacter::setAttackSpriteFrames(int outIndex, int inIndex, Vector2i origin, Vector2i bound) {
	attackSpriteOrigins[outIndex][inIndex] = origin;
	attackSpriteBounds[outIndex][inIndex] = bound;
}

void PlayerCharacter::setAttackSprite(int attackType) {
	sprite.setTextureRect(
		IntRect(
			attackSpriteOrigins[attackType][attackFrame].x,
			attackSpriteOrigins[attackType][attackFrame].y,
			attackSpriteBounds[attackType][attackFrame].x,
			attackSpriteBounds[attackType][attackFrame].y
		)
	);
	sprite.setOrigin(
		attackSpriteBounds[attackType][attackFrame].x / 2.0f,
		attackSpriteBounds[attackType][attackFrame].y / 2.0f
	);
}

void PlayerCharacter::setMoveSprite(int moveType) {
	if (spriteState == SpriteState::IDLE) {
		spriteState = SpriteState::MOVING;
		resetMoveFrame(moveType);
	}

	sprite.setTextureRect(
		IntRect(
			moveSpriteOrigins[moveType][moveFrame].x,
			moveSpriteOrigins[moveType][moveFrame].y,
			moveSpriteBounds[moveType][moveFrame].x,
			moveSpriteBounds[moveType][moveFrame].y
		)
	);
	sprite.setOrigin(
		moveSpriteBounds[moveType][moveFrame].x / 2.0f,
		moveSpriteBounds[moveType][moveFrame].y / 2.0f
	);
}
