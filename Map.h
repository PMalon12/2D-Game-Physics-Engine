#pragma once
#include "Include.h"
#include "Structures.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Grid.h"
#include "Rectangle.h"

#define MAXNODECAPACITY 5
#define MAXDEPTH 6

#define square(b) sq[0].setVertices(Vertex2D(-b.x / 2, -b.y / 2, 0, 0), Vertex2D(b.x / 2, b.y / 2, 1, 1), Vertex2D(-b.x / 2, b.y / 2, 0, 1)); sq[1].setVertices(Vertex2D(-b.x / 2, -b.y / 2, 0, 0), Vertex2D(b.x / 2, -b.y / 2, 1, 0), Vertex2D(b.x / 2, b.y / 2, 1, 1));
#define squaretex(b,t) sq[0].setVertices(Vertex2D(-b.x / 2, -b.y / 2, 0, 0), Vertex2D(b.x / 2, b.y / 2, (b.x/t.x), (b.y/t.y)), Vertex2D(-b.x / 2, b.y / 2, 0, (b.y/t.y))); sq[1].setVertices(Vertex2D(-b.x / 2, -b.y / 2, 0, 0), Vertex2D(b.x / 2, -b.y / 2, (b.x/t.x), 0), Vertex2D(b.x / 2, b.y / 2, (b.x/t.x), (b.y/t.y)));

template<class T>
class QuadTree
{
public:
	QuadTree() {}
	QuadTree(frect pBounds, unsigned short pDepth, unsigned short pMaxDepth, unsigned int pMaxNodeCapacity) 
		: bounds(pBounds), depth(pDepth), NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr), maxDepth(pMaxDepth), maxNodeCapacity(pMaxNodeCapacity) {}
	~QuadTree() {}

	void drawBounds()
	{
		glVertex3f(bounds.left, bounds.bot, 0.f);
		glVertex3f(bounds.left + bounds.width, bounds.bot, 0.f);

		glVertex3f(bounds.left + bounds.width, bounds.bot, 0.f);
		glVertex3f(bounds.left + bounds.width, bounds.bot + bounds.height, 0.f);

		glVertex3f(bounds.left + bounds.width, bounds.bot + bounds.height, 0.f);
		glVertex3f(bounds.left, bounds.bot + bounds.height, 0.f);

		glVertex3f(bounds.left, bounds.bot + bounds.height, 0.f);
		glVertex3f(bounds.left, bounds.bot, 0.f);

		if (NW != nullptr)
		{
			NW->drawBounds();
			NE->drawBounds();
			SW->drawBounds();
			SE->drawBounds();
		}
	}

	bool insert(T pStruct)
	{
		if (!bounds.intersects(ptr(pStruct)->getGlobalBounds()))
		{
			return false;
		}

		if (NW == nullptr)
		{
			if (structs.size() < maxNodeCapacity || depth == maxDepth)
			{
				structs.push_back(pStruct);
			}
			else
			{
				subdivide();
				NW->insert(pStruct);
				NE->insert(pStruct);
				SW->insert(pStruct);
				SE->insert(pStruct);
			}
		}
		else
		{
			NW->insert(pStruct);
			NE->insert(pStruct);
			SW->insert(pStruct);
			SE->insert(pStruct);
		}
		return true;
	}

	void subdivide()
	{
		NW = new QuadTree<T>(frect(bounds.left, bounds.bot, bounds.width / 2, bounds.height / 2), depth + 1, maxDepth, maxNodeCapacity);
		NE = new QuadTree<T>(frect(bounds.left + (bounds.width / 2), bounds.bot, bounds.width / 2, bounds.height / 2), depth + 1, maxDepth, maxNodeCapacity);
		SW = new QuadTree<T>(frect(bounds.left, bounds.bot + (bounds.height / 2), bounds.width / 2, bounds.height / 2), depth + 1, maxDepth, maxNodeCapacity);
		SE = new QuadTree<T>(frect(bounds.left + (bounds.width / 2), bounds.bot + (bounds.height / 2), bounds.width / 2, bounds.height / 2), depth + 1, maxDepth, maxNodeCapacity);

		for (auto itr = structs.begin(); itr != structs.end(); ++itr)
		{
			NW->insert((*itr));
			NE->insert((*itr));
			SW->insert((*itr));
			SE->insert((*itr));
		}

		structs.clear();
	}

	void query(frect pRegion, std::vector<T>& pDestList)
	{
		frect intersection;
		if (!pRegion.intersects(bounds, intersection))
		{
			return;
		}

		if (NW == nullptr)
		{
			if (intersection == bounds)
			{
				getAllObjects(pDestList);
				return;
			}

			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				pDestList.push_back(*itr);
			}
			return;
		}

		NW->query(pRegion, pDestList);
		NE->query(pRegion, pDestList);
		SW->query(pRegion, pDestList);
		SE->query(pRegion, pDestList);
	}

	void queryOnlyIntersecting(frect pRegion, std::vector<T>& pDestList)
	{
		frect intersection;
		if (!pRegion.intersects(bounds, intersection))
		{
			return;
		}

		if (NW == nullptr)
		{
			if (intersection == bounds)
			{
				getAllObjects(pDestList);
				return;
			}

			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				if (pRegion.intersects((*itr)->getGlobalBounds()))
				{
					pDestList.push_back(*itr);
				}
			}
			return;
		}

		NW->queryOnlyIntersecting(pRegion, pDestList);
		NE->queryOnlyIntersecting(pRegion, pDestList);
		SW->queryOnlyIntersecting(pRegion, pDestList);
		SE->queryOnlyIntersecting(pRegion, pDestList);
	}

	template<class R>
	void queryOnlyIntersecting(frect pRegion, std::vector<R>& pDestList)
	{
		frect intersection;
		if (!pRegion.intersects(bounds, intersection))
		{
			return;
		}

		if (NW == nullptr)
		{
			if (intersection == bounds)
			{
				getAllObjects(pDestList);
				return;
			}

			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				if (pRegion.intersects((*itr)->getGlobalBounds()))
				{
					pDestList.push_back(dynamic_cast<R>(*itr));
				}
			}
			return;
		}

		NW->queryOnlyIntersecting(pRegion, pDestList);
		NE->queryOnlyIntersecting(pRegion, pDestList);
		SW->queryOnlyIntersecting(pRegion, pDestList);
		SE->queryOnlyIntersecting(pRegion, pDestList);
	}

	void getAllObjects(std::vector<T>& pDestList)
	{
		if (NW == nullptr)
		{
			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				pDestList.push_back(*itr);
			}
		}
		else
		{
			NW->getAllObjects(pDestList);
			NE->getAllObjects(pDestList);
			SW->getAllObjects(pDestList);
			SE->getAllObjects(pDestList);
		}
	}

	template<class R>
	void getAllObjects(std::vector<R>& pDestList)
	{
		if (NW == nullptr)
		{
			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				pDestList.push_back(dynamic_cast<R>(*itr));
			}
		}
		else
		{
			NW->getAllObjects(pDestList);
			NE->getAllObjects(pDestList);
			SW->getAllObjects(pDestList);
			SE->getAllObjects(pDestList);
		}
	}

	void queryDraw(frect pRegion, float alpha)
	{
		frect intersection;
		if (!pRegion.intersects(bounds, intersection))
		{
			return;
		}

		if (intersection == bounds)
		{
			drawAll(alpha);
			return;
		}

		if (NW == nullptr)
		{
			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				(*itr)->draw(alpha);
			}

			return;
		}

		NW->queryDraw(pRegion, alpha);
		NE->queryDraw(pRegion, alpha);
		SW->queryDraw(pRegion, alpha);
		SE->queryDraw(pRegion, alpha);
	}

	void drawAll(float alpha)
	{
		if (NW == nullptr)
		{
			for (auto itr = structs.begin(); itr != structs.end(); ++itr)
			{
				(*itr)->draw(alpha);
			}
			return;
		}
		else
		{
			NW->drawAll(alpha);
			NE->drawAll(alpha);
			SW->drawAll(alpha);
			SE->drawAll(alpha);
		}
	}

	void clear()
	{
		if (NW != nullptr)
		{
			NW->clear();
			NE->clear();
			SW->clear();
			SE->clear();
			delete NW; delete NE; delete SW; delete SE;
			NW = nullptr; NE = nullptr; SW = nullptr; SE = nullptr;
		}
		structs.clear();
	}

	void update(std::vector<T>& updateList)
	{
		clear();

		for (auto itr = updateList.begin(); itr != updateList.end(); ++itr)
		{
			insert(*itr);
		}
	}

	void updatePhysics()
	{
		std::vector<Physical*>* tmp = reinterpret_cast<std::vector<Physical*>*>(&structs);
		Collector::collectList(*tmp);

		if (NW != nullptr)
		{
			NW->updatePhysics();
			NE->updatePhysics();
			SW->updatePhysics();
			SE->updatePhysics();
		}
	}

	unsigned short depth;

	frect bounds;

	QuadTree<T> *NW, *NE, *SW, *SE;

	std::vector<T> structs;

private:

	unsigned int maxNodeCapacity;
	unsigned short maxDepth;

};

typedef QuadTree<Entity*> EQuadTree;
typedef QuadTree<StaticStructure*> SQuadTree;
typedef QuadTree<DynamicStructure*> DQuadTree;

class Player;
class Camera;

class Map
{
	friend class Game;
	friend class Console;
private:
	enum MapMode { Play, Edit };
	enum PlaceMode { SS, DS, Lava, CHKPoint, EndLvl, PWRup };

	struct Properties
	{
		char mapName[64];
		vec2f size;
		vec2f gravity;
	} properties;

	GLuint modelUniform;
	GLuint corrUniform;

	Shader* shader;
	Texture* backTexture;
	vec2f backTextureZoom;
	GLuint backVBO;
	GLuint backVAO;

	MapMode editMode;
	PlaceMode placeMode;
	std::vector<Physical*> addHistory;
	RectangleShape toAdd;
	Transform placeTransform;
	vec2f placeGranularity;
	vec2f scaleGranularity;
	float rotateGranularity;
	Camera* currentCamera;

	Text editHelp;
	Text editModeText;
	RectangleShape editModeEx;

	std::string mapPath;
	Player* player;

	int checkPointNum;
	vec2f checkPointPos;
	vec2f startPos;

	std::vector<StaticStructure*> structures;
	std::vector<StaticStructure*> structuresToAdd;
	std::vector<StaticStructure*> structuresToRemove;

	std::vector<DynamicStructure*> dynamics;
	std::vector<DynamicStructure*> dynamicsToAdd;
	std::vector<DynamicStructure*> dynamicsToRemove;

	std::vector<Entity*> entities;
	std::vector<Entity*> entitiesToRemove;
	std::vector<Entity*> entitiesToAdd;

	std::map<Physical*, Physical*> rigids;

	Checkpoint* lastCheckpoint;

public:

	SQuadTree sQuadTree;
	DQuadTree dQuadTree;
	EQuadTree eQuadTree;
	Grid physicalBodies;

	Map(vec2f pSize);
	Map(std::string pMapName);
	~Map();

	void draw(float a);
	void update(float dt);

	void loadMap(std::string pMapName);
	void saveMap(std::string pMapName);

	void addSS(StaticStructure* ss)
	{
		structuresToAdd.push_back(ss);
	}

	void addStatic(StaticStructure* ss)
	{
		structuresToAdd.push_back(ss);
	}

	void addDynamic(DynamicStructure* ds)
	{
		dynamicsToAdd.push_back(ds);
	}

	void addEntity(Entity* e)
	{
		entitiesToAdd.push_back(e);
	}

	void removeStructure(StaticStructure* s)
	{
		structuresToRemove.push_back(s);
	}

	void removeDynamic(DynamicStructure* s)
	{
		dynamicsToRemove.push_back(s);
	}

	void removeEntity(Entity* e)
	{
		entitiesToRemove.push_back(e);
	}

	Physical* getObject(int32_t id)
	{
		for (auto itr = structures.begin(); itr != structures.end(); ++itr)
		{
			if ((*itr)->getUID() == id)
			{
				return *itr;
			}
		}
		for (auto itr = dynamics.begin(); itr != dynamics.end(); ++itr)
		{
			if ((*itr)->getUID() == id)
			{
				return *itr;
			}
		}
		for (auto itr = entities.begin(); itr != entities.end(); ++itr)
		{
			if ((*itr)->getUID() == id)
			{
				return *itr;
			}
		}
		return nullptr;
	}

	void removeObject(int32_t id)
	{
		auto obj = getObject(id);
		if (obj == nullptr) { return; }
		for (auto itr = addHistory.begin(); itr != addHistory.end(); ++itr)
		{
			if (*itr == obj)
			{
				addHistory.erase(itr);
				break;
			}
		}
		switch (obj->getID())
		{
		case O_ENTITY:
			removeEntity((Entity*)obj);
			break;
		case O_STATIC_STRUCT:
			removeStructure((StaticStructure*)obj);
			break;
		case O_DYNAMIC_STRUCT:
			removeDynamic((DynamicStructure*)obj);
			break;
		}
	}

	void handleRoguePointer(Physical* ptr)
	{
		for (auto itr = entities.begin(); itr != entities.end(); ++itr)
		{
			for (auto itrr = (*itr)->contactManifolds.begin(); itrr != (*itr)->contactManifolds.end();)
			{
				if (itrr->A == ptr || itrr->B == ptr)
				{
					std::cout << "HANDLING ROGUE POINTER" << std::endl;
					itrr = (*itr)->contactManifolds.erase(itrr);
				}
				else
					++itrr;
			}
		}

		physicalBodies.clear();

		for (auto itrrr = rigids.begin(); itrrr != rigids.end(); itrrr++)
		{
			itrrr->first->contactManifolds.clear();
			itrrr->first->involvedContactManifolds.clear();

			physicalBodies.updateBucketsForObject(itrrr->first);
		}
	}

	Player* getPlayer() const { return player; }

	Properties& const getProperties()
	{
		return properties;
	}

	Entity* getEntity(uint32_t id)
	{
		for (auto itr = entities.begin(); itr != entities.end(); ++itr)
		{
			if ((*itr)->getUID() == id)
			{
				return *itr;
			}
		}
	}

	void toggleEditMode();
	Camera* getCurrentCamera() { return currentCamera; }
	void setCurrentCamera(Camera* cam) { currentCamera = cam; }

	bool checkpointTrigger(int index, vec2f pos, Checkpoint* structure)
	{
		if (index >= checkPointNum)
		{
			if (lastCheckpoint != nullptr)
			{
				lastCheckpoint->mesh->r = 1.0;
				lastCheckpoint->mesh->g = 0.0;
			}
			checkPointNum = index;
			checkPointPos = pos;
			lastCheckpoint = structure;
			return true;
		}
		return false;
	}
	void setCheckpointIndex(Checkpoint* structure, int index)
	{
		structure->setIndex(index);
	}
	void resetCheckpointInfo()
	{
		checkPointNum = -1;
		checkPointPos = vec2f(0, 0);
	}

	void setBackGroundTexture(Texture* tex, vec2f scale = vec2f(1, 1))
	{
		backTexture = tex;
		backTextureZoom = scale;
	}

private:

	uint32_t getNextID() { return entities.size() + structures.size() + dynamics.size() + entitiesToAdd.size() + structuresToAdd.size() + dynamicsToAdd.size(); }

	void createMapBorder(float width, float outsideWidth)
	{
		auto col = [](StaticStructure* ss, float r, float g, float b, float a) -> void { ss->mesh->r = r; ss->mesh->g = g; ss->mesh->b = b; ss->mesh->a = a; };

		Triangle* tris = new Triangle[2];

		tris[0].setVertices(Vertex(-outsideWidth, -outsideWidth), Vertex(properties.size.x - width, width), Vertex(width, width));
		tris[1].setVertices(Vertex(-outsideWidth, -outsideWidth), Vertex(properties.size.x + outsideWidth, -outsideWidth), Vertex(properties.size.x - width, width));

		StaticStructure* bot = new StaticStructure(new TriangleMesh(2, tris)); bot->setStatic(); bot->setUID(getNextID());
		addSS(bot);

		tris[0].setVertices(Vertex(-outsideWidth, -outsideWidth), Vertex(width, properties.size.y - width), Vertex(-outsideWidth, properties.size.y + outsideWidth));
		tris[1].setVertices(Vertex(-outsideWidth, -outsideWidth), Vertex(width, width), Vertex(width, properties.size.y - width));

		StaticStructure* left = new StaticStructure(new TriangleMesh(2, tris)); left->setStatic(); left->setUID(getNextID());
		addSS(left);

		tris[0].setVertices(Vertex(properties.size.x + outsideWidth, -outsideWidth), Vertex(properties.size.x - width, properties.size.y - width), Vertex(properties.size.x - width, width));
		tris[1].setVertices(Vertex(properties.size.x + outsideWidth, -outsideWidth), Vertex(properties.size.x + outsideWidth, properties.size.y + outsideWidth), Vertex(properties.size.x - width, properties.size.y - width));

		StaticStructure* right = new StaticStructure(new TriangleMesh(2, tris)); right->setStatic(); right->setUID(getNextID());
		addSS(right);

		tris[0].setVertices(Vertex(properties.size.x + outsideWidth, properties.size.y + outsideWidth), Vertex(-outsideWidth, properties.size.y + outsideWidth), Vertex(width, properties.size.y - width));
		tris[1].setVertices(Vertex(properties.size.x + outsideWidth, properties.size.y + outsideWidth), Vertex(width, properties.size.y - width), Vertex(properties.size.x - width, properties.size.y - width));

		StaticStructure* top = new StaticStructure(new TriangleMesh(2, tris)); top->setStatic(); top->setUID(getNextID());
		addSS(top);

		col(bot, 0, 0, 0, 1);
		col(left, 0, 0, 0, 1);
		col(right, 0, 0, 0, 1);
		col(top, 0, 0, 0, 1);
	}

	void updateBackRect();

	void clear()
	{
		checkPointNum = -1; lastCheckpoint = nullptr;
		physicalBodies.clear();
		physicalBodies.manifolds.clear();
		sQuadTree.clear();
		dQuadTree.clear();
		eQuadTree.clear();

		for (auto itr = structures.begin(); itr != structures.end(); ++itr)
		{
			delete (*itr);
		}

		for (auto itr = dynamics.begin(); itr != dynamics.end(); ++itr)
		{
			delete (*itr);
		}

		for (auto itr = entities.begin(); itr != entities.end(); ++itr)
		{
			delete (*itr);
		}

		entities.clear();
		structures.clear();
		dynamics.clear();
		rigids.clear();
	}

};
