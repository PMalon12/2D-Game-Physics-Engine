#pragma once
#include "Include.h"
#include "RigidBody.h"
#include "Polygon.h"
#include "Transform.h"
#include "Grid.h"
#include "Manifold.h"
#include "Text.h"

class DeletedObjectEx
{
public:
	DeletedObjectEx(Physical* id)
	{
		message = "Error, object " + std::to_string(std::intptr_t(id)) + " deleted.";
		obj = id;
	}

	std::string message;
	Physical* obj;
};

class Map;

class Physical : public RigidBody, public Transformable
{
public:
	Physical() { affectedByGravity = true; density = 0.01; }
	Physical(std::ifstream& ifs);
	~Physical()
	{
		for (auto itr = contactManifolds.begin(); itr != contactManifolds.end(); ++itr)
			itr->valid = false;

		for (auto itr = involvedContactManifolds.begin(); itr != involvedContactManifolds.end(); ++itr)
			itr->valid = false;

		contactManifolds.clear();

		for (auto itr = involvedContactManifolds.begin(); itr != involvedContactManifolds.end(); ++itr) {
			itr->deleteFromOtherObject(this);
		}
	}

	TriangleMesh* mesh;
	unsigned int noMesh;
	float* collisionVerts;
	size_t collisionVertsSize;

	virtual void init() {}

	vec2f getSupportPoint(vec2f direction)
	{
		float* set;
		int setSize = getVertices(set);

		glm::fvec3 t(direction.x, direction.y, 1.f);
		t = t * getTransformMat();
		direction.x = t.x; direction.y = t.y;
		
		vec2f next(set[0], set[1]);
		t = glm::fvec3(next.x, next.y, 1.f);

		float max = vec2f(t.x, t.y).dot(direction);
		int maxI = 0;
		for (size_t i = 2; i < (setSize); i += 2)
		{
			next = vec2f(set[i], set[i + 1]);
			t = glm::fvec3(next.x, next.y, 1.f);

			float dot = vec2f(t.x, t.y).dot(direction);
			if (dot > max)
			{
				max = dot;
				maxI = i;
			}
		}

		vec2f ret(set[maxI], set[maxI + 1]);
		glm::fvec3 ret2(ret.x, ret.y, 1.f);
		ret2 = getTransformMat() * ret2;
		ret.x = ret2.x; ret.y = ret2.y;

		return ret;
	}

	vec2f getSupportPoint(vec2f direction, int& index)
	{
		float* set;
		int setSize = getVertices(set);

		glm::fvec3 t(direction.x, direction.y, 1.f);
		t = t * getTransformMat();
		direction.x = t.x; direction.y = t.y;

		vec2f next(set[0], set[1]);
		t = glm::fvec3(next.x, next.y, 1.f);

		float max = vec2f(t.x, t.y).dot(direction);
		int maxI = 0;
		for (size_t i = 2; i < (setSize); i += 2)
		{
			next = vec2f(set[i], set[i + 1]);
			t = glm::fvec3(next.x, next.y, 1.f);

			float dot = vec2f(t.x, t.y).dot(direction);
			if (dot > max)
			{
				max = dot;
				maxI = i;
			}
		}

		vec2f ret(set[maxI], set[maxI + 1]);
		glm::fvec3 ret2(ret.x, ret.y, 1.f);
		ret2 = getTransformMat() * ret2;
		ret.x = ret2.x; ret.y = ret2.y;
		index = maxI;

		return ret;
	}

	void collisionCallback(Physical* collider, const ContactManifold& manifold)
	{
		if (collisionCallbacks.size() > 2) {
			throw DeletedObjectEx(this);
		}
	
		for (auto itr = collisionCallbacks.begin(); itr != collisionCallbacks.end(); ++itr)
		{
			(*itr).first.operator()(this, collider, manifold);
		}
	}

	bool allowCollision(const Physical* collider, const ContactManifold& manifold)
	{
		if (collisionCriteria.size() > 2) {
			throw DeletedObjectEx(this);
			return false;
		}

		if (collisionCriteria.size() == 0)
		{
			return getBit(getID(), PHYSICAL_BIT);
		}

		bool result = true;
		for (auto itr = collisionCriteria.begin(); itr != collisionCriteria.end(); ++itr)
		{
			result = result && (*itr).first.operator()(this, collider, manifold);
		}
		return result;
	}



	void deregisterCollisionCallbacks()
	{
		collisionCallbacks.clear();
	}
	bool registerCollisionCallback(CollisionCallback& callback)
	{
		collisionCallbacks.push_back(std::make_pair(callback,"callback")); return true;
	}

	void deregisterCollisionCriteria()
	{
		collisionCriteria.clear();
	}
	bool registerCollisionCriteria(CollisionCriteria& criteria)
	{
		collisionCriteria.push_back(std::make_pair(criteria,"criteria")); return true;
	}

	std::vector<std::pair<CollisionCallback, std::string>> collisionCallbacks;
	std::vector<std::pair<CollisionCriteria, std::string>> collisionCriteria;

	unsigned int getVertices(float*& pVert)
	{
		if (collisionVerts != nullptr)
		{
			pVert = collisionVerts;
			return collisionVertsSize;
		}
		if (mesh->edgeVertData != nullptr)
		{
			pVert = mesh->edgeVertData;
			return mesh->edgeVertDataSize;
		}

		pVert = mesh->trianglesRaw;
		return mesh->trianglesRawSize;
	}

	virtual void update(float dt) = 0;
	void updatePhysics();
	void dampen(float x);
	void drawBounds(Transform& t);
	void drawID(Transform& t);

	void setTexture(Texture* t)
	{
		mesh->texture = t;
	}

	frect getLocalBounds() { return localBounds; }
	frect getGlobalBounds() { return frect(localBounds.left + getPosition().x, localBounds.bot + getPosition().y, localBounds.width, localBounds.height); }

	Map* map;
	std::set<ContactManifold> contactManifolds;
	std::set<ContactManifold> involvedContactManifolds;

	Grid::ObjectBucketIDs getBucketIDs() const { return bucketIDs; }
	void setBucketIDs(Grid::ObjectBucketIDs p) { bucketIDs = p; }

	void calculateMassAndInertia()
	{
		float area = 0;
		const vec2f* verts = reinterpret_cast<vec2f*>(mesh->edgeVertData);
		const size_t num = mesh->edgeVertDataSize * 0.5f;
		for (size_t i = 0; i < num - 1; ++i)
		{
			area += (verts[i].x * verts[i + 1].y);
		}
		area += (verts[num - 1].x * verts[0].y);
		for (size_t i = 0; i < num - 1; ++i)
		{
			area -= (verts[i + 1].x * verts[i].y);
		}
		area -= (verts[0].x * verts[num - 1].y);
		area = std::abs(area) * 0.5f;

		setBodyMass(area * density);

		float p1 = 0, p2 = 0;
		for (size_t i = 0; i < num - 1; ++i)
		{
			p1 += abs(verts[i + 1].cross(verts[i])*(verts[i].dot(verts[i]) + (verts[i].dot(verts[i + 1])) + (verts[i + 1].dot(verts[i + 1]))));
		}
		p1 += abs(verts[0].cross(verts[num - 1])*(verts[num - 1].dot(verts[num - 1]) + (verts[num - 1].dot(verts[0])) + (verts[0].dot(verts[0]))));
		for (size_t i = 0; i < num - 1; ++i)
		{
			p2 += abs(verts[i + 1].cross(verts[i]));
		}
		p2 += abs(verts[0].cross(verts[num - 1]));

		setBodyInertiaMoment((getBodyMass() * p1) / (6 * p2));
	}

	void calculateMassAndInertia(float dens)
	{
		density = dens;
		calculateMassAndInertia();
	}

	inline void setPosition(vec2f pPos)
	{
		setBodyPosition(pPos); Transformable::setPosition(pPos);
		contactManifolds.clear();
		for (auto itr = involvedContactManifolds.begin(); itr != involvedContactManifolds.end(); ++itr)
		{
			(*itr).deleteFromObjects();
		}
	}

	inline void setRotation(float pRot) { setBodyRotation(pRot); Transformable::setRotation(pRot); }

	void setUID(uint32_t pUID);
	void setID(uint32_t pID) { ID = pID; }
	uint32_t getID() const { return ID; }
	uint32_t getUID() const { return UID; }
	void setIDBit(uint8_t bit, uint8_t val)
	{
		ID ^= (-val ^ ID) & (1 << bit);
	}

	void updateBounds()
	{
		float cosTheta = std::cos(getRotation());
		float sinTheta = std::sin(getRotation());
		vec2f scale = getScale();

		auto getX = [this, cosTheta, sinTheta, scale](int i) -> float
		{
			return (mesh->trianglesRaw[i] * cosTheta * scale.x) - (mesh->trianglesRaw[i + 1] * sinTheta * scale.y);
		};

		auto getY = [this, cosTheta, sinTheta, scale](int i) -> float
		{
			return (mesh->trianglesRaw[i - 1] * sinTheta * scale.x) + (mesh->trianglesRaw[i] * cosTheta * scale.y);
		};

		std::pair<float, float> minMaxX;
		minMaxX.first = getX(0);
		minMaxX.second = getX(0);
		std::pair<float, float> minMaxY;
		minMaxY.first = getY(1);
		minMaxY.second = getY(1);

		for (int i = 2; i < mesh->trianglesRawSize; i += 2)
		{
			auto nextX = getX(i);
			if (nextX < minMaxX.first)
			{
				minMaxX.first = nextX;
			}
			else if (nextX > minMaxX.second)
			{
				minMaxX.second = nextX;
			}
		}

		for (int i = 3; i < mesh->trianglesRawSize; i += 2)
		{
			auto nextY = getY(i);
			if (nextY < minMaxY.first)
			{
				minMaxY.first = nextY;
			}
			else if (nextY > minMaxY.second)
			{
				minMaxY.second = nextY;
			}
		}

		localBounds = frect(minMaxX.first, minMaxY.first, (minMaxX.second - minMaxX.first), (minMaxY.second - minMaxY.first));
		uid.setPosition(getBodyPosition() + getOrigin());
	}

	bool getAffectedByGravity() { return affectedByGravity; }
	void setAffectedByGravity(bool p) { affectedByGravity = p; }

	void setDensity(float pd) { density = pd; }

protected:

	Grid::ObjectBucketIDs bucketIDs;
	float density;

	frect localBounds;
	
	vec2f prevPos;
	float prevRot;

	Text uid;

	virtual void loadFromStream(std::ifstream& ifs)
	{
		loadTransformFromStream(ifs);
		ifs.read((char*)&UID, sizeof(UID));
		ifs.read((char*)&state, sizeof(state));
		ifs.read((char*)&staticFriction, sizeof(staticFriction));
		ifs.read((char*)&dynamicFriction, sizeof(dynamicFriction));
		ifs.read((char*)&restitution, sizeof(restitution));
		ifs.read((char*)&affectedByGravity, sizeof(affectedByGravity));
		ifs.read((char*)&collisionVertsSize, sizeof(collisionVertsSize));
		if (collisionVertsSize != 0) {
			collisionVerts = new float[collisionVertsSize];
			ifs.read((char*)collisionVerts, collisionVertsSize * sizeof(float));
		}
	}

	virtual void saveToStream(std::ofstream& ofs)
	{
		saveTransformToStream(ofs);
		ofs.write((char*)&UID, sizeof(UID));
		ofs.write((char*)&state, sizeof(state));
		ofs.write((char*)&staticFriction, sizeof(staticFriction));
		ofs.write((char*)&dynamicFriction, sizeof(dynamicFriction));
		ofs.write((char*)&restitution, sizeof(restitution));
		ofs.write((char*)&affectedByGravity, sizeof(affectedByGravity));
		ofs.write((char*)&collisionVertsSize, sizeof(collisionVertsSize));
		ofs.write((char*)collisionVerts, collisionVertsSize * sizeof(float));
	}

private:
	uint32_t UID;
	uint32_t ID;
};