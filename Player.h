#pragma once
#include "Include.h"
#include "Character.h"
#include "Ray.h"
#include "Structures.h"
#include "Map.h"
#include "Text.h"
#include "GJK.h"

#define MAXWALKSLOPEPOS vec2f(1.f,1.f)
#define MAXWALKSLOPENEG vec2f(1.f,-1.f)

class Player : public Character
{
	friend class EventManager;
	friend class Map;
public:
	Player();
	Player(std::ifstream& ifs);
	~Player();

	void update(float dt);
	void jump();
	void doubleJump();
	void move(vec2f a);
	void fire();

	void initt();
	void registerInputCallbacks();
	void deregisterInputCallbacks();

	void draw(float alpha);

	void saveToStream(std::ofstream& ofs);
	void loadFromStream(std::ifstream& ifs);

	int getCurItem() const { return curItem; }

	void setJumpStrength(float str) { jumpStr = str; }

	void givePower(uint8_t power)
	{
		hasPower[power] = true;
	}

private:

	int curItem;

	float movForce;
	float jumpStr;
	enum Face { Left, Right };
	Face texFace;

	bool hasPower[6];

	float fm;
	vec2f gm;
	vec2f steepest;

	Physical* heldObject;
	Text debug;

	TriangleMesh* glow;
	TriangleMesh* col;

	bool spaceReleasedAfterJump;


	struct Checkpoint
	{
		int index;
		StaticStructure* structure;
	};
	std::vector<Checkpoint> checkpoints;
};
