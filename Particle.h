#pragma once
#include "Include.h"
#include "Texture.h"
#include "Utility.cpp"
#include "Entity.h"

class Particle :public Entity
{
	Particle(Texture* a, float rad);
	Particle(std::ifstream& ifs) : Entity(ifs)
	{
		Particle::loadFromStream(ifs);
	}
	~Particle() {}

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
		ofs.write((char*)&dead, sizeof(dead));
		ofs.write((char*)&lifeLeft, sizeof(lifeLeft));
	}

	bool dead;
	Time lifeLeft;

private:

	void loadFromStream(std::ifstream& ifs)
	{
		ifs.read((char*)&dead, sizeof(dead));
		ifs.read((char*)&lifeLeft, sizeof(lifeLeft));
	}
};