#pragma once
#include "Include.h"
#include "Followable.h"
#include "Text.h"
#include "RigidBody.h"

#include "Physical.h"

#include "Texture.h"

class Entity : public Physical
{
	friend class EventManager;
public:
	Entity() {}
	Entity(std::ifstream& ifs) : Physical(ifs)
	{
		Entity::loadFromStream(ifs);
	}
	virtual ~Entity() { 
		delete mesh;
	}

	virtual void update(float dt) = 0;
	virtual void draw(float alpha) = 0;

	virtual void setTexture(Texture* t)
	{
		mesh->texture = t;
	}

	virtual void saveToStream(std::ofstream& ofs);

protected:

	virtual void loadFromStream(std::ifstream& ifs);
};