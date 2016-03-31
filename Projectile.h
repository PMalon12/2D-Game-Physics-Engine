#pragma once
#include "Entity.h"
#include "Utility.cpp"

class Projectile : public Entity
{
public:
	Projectile(Texture* a, float rad);
	Projectile(std::ifstream& ifs) : Entity(ifs) 
	{
		Projectile::loadFromStream(ifs); 
	}
	~Projectile() {}

	void update(float dt);

	void draw(float alpha) 
	{ 
		vec2f corPos = vlerp(prevPos, getBodyPosition(), alpha);
		float corRot = lerp(prevRot, getBodyRotation(), alpha);

		auto x = getTransform();
		x.setTranslation(corPos);
		x.setRotation(corRot);

		mesh->draw(x);

		mesh->drawConvexHull(x);
	}

	void saveToStream(std::ofstream& ofs) 
	{
		Entity::saveToStream(ofs);
		auto parentUID = parent->getUID();
		ofs.write((char*)&parentUID, sizeof(parentUID));
		ofs.write((char*)&dead, sizeof(dead));
		ofs.write((char*)&lifeLeft, sizeof(lifeLeft));
	}

	Entity* parent;
	uint32_t parUID;
	bool dead;
	Time lifeLeft;

	void init();

private:

	void loadFromStream(std::ifstream& ifs) 
	{
		ifs.read((char*)&parUID, sizeof(parUID));
		ifs.read((char*)&dead, sizeof(dead));
		ifs.read((char*)&lifeLeft, sizeof(lifeLeft));
	}
};