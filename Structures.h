#pragma once
#include "Include.h"
#include "Polygon.h"
#include "Physical.h"

class Structure : public Physical
{
	friend class Map;
public:
	Structure() {}
	Structure(std::ifstream& ifs) : Physical(ifs)
	{
		Structure::loadFromStream(ifs);
	}
	Structure(TriangleMesh* pTMesh)
	{
		mesh = pTMesh;
		noMesh = 1;

		setID(O_STRUCTURE);

		updateBounds();
	}

	~Structure() 
	{
		delete mesh;
	}

	virtual void draw(float alpha);
	virtual void update(float dt) {}

	TriangleMesh* getMesh() { return mesh; }

	//void collisionCallback(const Physical* collider, const ContactManifold& manifold) {}
	//bool allowCollision(const Physical* collider) { return true; }

protected:

	

	virtual void saveToStream(std::ofstream& ofs) 
	{
		Physical::saveToStream(ofs);
		mesh->saveToStream(ofs);
	}

	virtual void loadFromStream(std::ifstream& ifs)
	{
		mesh = new TriangleMesh(ifs);
	}
};

class StaticStructure : public Structure
{
public:
	StaticStructure() {}

	StaticStructure(std::ifstream& ifs) : Structure(ifs)
	{
		//loadFromStream(ifs);
		updateBounds();
		setStatic();
	}

	StaticStructure(TriangleMesh* pTMesh) : Structure(pTMesh) 
	{
		setID(O_STATIC_STRUCT);
	}

	~StaticStructure() {}

	virtual void draw(float a) { Structure::draw(a); }
};

class LavaStructure : public StaticStructure
{
public:
	LavaStructure(std::ifstream& ifs);
	LavaStructure(TriangleMesh* pTMesh);
	~LavaStructure() {}

	void draw(float a);
};

class ButtonStructure : public StaticStructure
{
public:
	ButtonStructure(std::ifstream& ifs) : StaticStructure(ifs) {}
	ButtonStructure(TriangleMesh* pTMesh) : StaticStructure(pTMesh) {}
	~ButtonStructure() {}

	virtual void update(float dt) {}
};

class EndLevel : public StaticStructure
{
public:
	EndLevel(std::ifstream& ifs);
	EndLevel(TriangleMesh* pTMesh);
	~EndLevel() {}

	void setNextLevelName(std::string pName)
	{
		std::memcpy(nextLevel, pName.c_str(), std::min(pName.size(),size_t(32)));
	}

	virtual void saveToStream(std::ofstream& ofs)
	{
		StaticStructure::saveToStream(ofs);
		ofs.write(nextLevel, sizeof(nextLevel));
	}

private:
	virtual void loadFromStream(std::ifstream& ifs)
	{
		ifs.read(nextLevel, sizeof(nextLevel));
	}

	char nextLevel[32];
};

class Checkpoint : public StaticStructure
{
public:
	Checkpoint(std::ifstream& ifs) : StaticStructure(ifs) 
	{
		loadFromStream(ifs);
		setID(O_CHECKPOINT); 
		static CollisionCriteria cr = [](Physical* __this, const Physical* collider, const ContactManifold& manifold) -> bool
		{
			return false;
		};
		mesh->r = 0.1f;
		registerCollisionCriteria(cr);
	}
	Checkpoint(TriangleMesh* pTMesh) : StaticStructure(pTMesh) 
	{
		setID(O_CHECKPOINT); checkpointIndex = -1; 
		static CollisionCriteria cr = [](Physical* __this, const Physical* collider, const ContactManifold& manifold) -> bool
		{
			return false;
		};
		mesh->r = 0.1f;
		registerCollisionCriteria(cr);
	}
	~Checkpoint() {}

	void setIndex(int i);

	void draw(float a);

	int checkpointIndex;

	virtual void saveToStream(std::ofstream& ofs)
	{
		StaticStructure::saveToStream(ofs);
		ofs.write((char*)(&checkpointIndex), sizeof(checkpointIndex));
	}

private:
	virtual void loadFromStream(std::ifstream& ifs)
	{
		ifs.read((char*)(&checkpointIndex), sizeof(checkpointIndex));
		setIndex(checkpointIndex);
	}
};

class DynamicStructure : public Structure
{
public:
	DynamicStructure(std::ifstream& ifs) : Structure(ifs) { setID(O_DYNAMIC_STRUCT); }
	DynamicStructure(TriangleMesh* pTMesh) : Structure(pTMesh) { setID(O_DYNAMIC_STRUCT); }
	~DynamicStructure() {}

	void draw(float a);
	void update(float dt);
};

class Powerup : public StaticStructure
{
public:
	Powerup();
	Powerup(std::ifstream& ifs);
	~Powerup() {}

	void draw(float alpha);
	void update(float dt) {}

	virtual void saveToStream(std::ofstream& ofs)
	{
		Structure::saveToStream(ofs);
		ofs.write((char*)&powerCode, sizeof(powerCode));
	}

	uint8_t powerCode;

private:

	virtual void loadFromStream(std::ifstream& ifs)
	{
		ifs.read((char*)&powerCode, sizeof(powerCode));
	}
};
