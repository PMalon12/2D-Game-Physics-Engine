#pragma once
#include "include.h"
#include "Entity.h"

class Character : public Entity
{
public:
	Character();
	Character(std::ifstream& ifs) : Entity(ifs) { loadFromStream(ifs); }
	virtual ~Character();

	virtual void update(float dt) = 0;

	virtual void draw(float alpha);

	virtual void saveToStream(std::ofstream& ofs)
	{
		Entity::saveToStream(ofs);
		ofs.write((char*)&health, sizeof(health));
		auto nameSize = name.size();
		ofs.write((char*)&nameSize, sizeof(nameSize));
		ofs.write(name.c_str(), nameSize);
	}

	virtual void loadFromStream(std::ifstream& ifs)
	{
		ifs.read((char*)&health, sizeof(health));
		size_t nameSize;
		ifs.read((char*)&nameSize, sizeof(nameSize));
		char* iname = new char[nameSize];
		ifs.read(iname, nameSize);
		name.assign(iname, nameSize);
		delete iname;
	}

	void damage(float dmg)
	{
		health -= dmg;
	}

	virtual bool canJump() { return !jumpLock; }

	float getHealth() const { return health; }
	void setHealth(float pHP) { health = pHP; }

	void setJumpLock(bool state) { jumpLock = state; }
	void setOnGround(bool state) { onGround = state; }

protected:

	std::string name;
	float health;
	
	bool jumpLock;

	bool onGround;
	float jumpResetDelay;
	float jumpResetDelayTimer;

	float jumpCD;
	float jumpTimer;

	bool primeDoubleJump;
	float doubleJumpCD;
	float doubleJumpTimer;
};
