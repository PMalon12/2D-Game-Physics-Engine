#pragma once
#include "Grid.h"
#include "Physical.h"
#include "Game.h"

void Grid::updateBucketsForObject(Physical* obj)
{
	ObjectBucketIDs ret;

	frect bounds = obj->getGlobalBounds();

	ret = obj->getBucketIDs();

	for (int x = ret.fx; x < ret.lx; ++x)
	{
		for (int y = ret.fy; y < ret.ly; ++y)
		{
			int i = x + (y * resolution.x);
			buckets[i].erase(obj);
		}
	}

	auto first = std::max(int(bounds.left / cellSize.x), 0) + (std::max(int(bounds.bot / cellSize.y), 0) * resolution.x);
	auto last = std::min(int((bounds.left + bounds.width) / cellSize.x), resolution.x - 1) + (std::min(int((bounds.bot + bounds.height) / cellSize.y), resolution.y - 1) * resolution.x);

	ret.fx = first % resolution.x;
	ret.fy = int(first / resolution.x);
	ret.lx = last % resolution.x + 1;
	ret.ly = int(last / resolution.x) + 1;

	for (int x = ret.fx; x < ret.lx; ++x)
	{
		for (int y = ret.fy; y < ret.ly; ++y)
		{
			unsigned int i = x + (y * resolution.x);
			if (buckets[i].find(obj) == buckets[i].end())
			{
				buckets[i].insert(std::make_pair(obj, obj));
			}
		}
	}

	obj->setBucketIDs(ret);
}

void Grid::removeObject(Physical * obj)
{
	//int xFirst = obj->getBucketIDs().fx;
	//int yFirst = obj->getBucketIDs().fy;
	//int xLast = obj->getBucketIDs().lx;
	//int yLast = obj->getBucketIDs().ly;

	////GET BUCKET INDEX FOR EACH BUCKET OBJECT IS IN
	//for (int x = xFirst; x < xLast; ++x)
	//{
	//	for (int y = yFirst; y < yLast; ++y)
	//	{
	//		int i = x + (y * Grid::res.x);
	//		buckets[i].erase(obj);
	//	}
	//}

	for (int i = 0; i < res.x * res.y; ++i)
	{
		buckets[i].erase(obj);
	}

	//for (int x = 0; x < res.x - 1; ++x)
	//{
	//	for (int y = 0; y < res.y - 1; ++y)
	//	{
	//		int i = x + (y * Grid::res.x);
	//		buckets[i].erase(obj);
	//	}
	//}
}

void Grid::drawBounds()
{
	float x = 0;

	for (int i = 0; i < resolution.x + 1; ++i)
	{
		glVertex3f(i * cellSize.x, 0, 0);
		glVertex3f(i * cellSize.x, size.y, 0);
	}

	float y = 0;

	for (int i = 0; i < resolution.y + 1; ++i)
	{
		glVertex3f(0, i * cellSize.y, 0);
		glVertex3f(size.x, i * cellSize.y, 0);
	}
}

void Grid::resolveManifolds()
{
	for (auto itr = manifolds.begin(); itr != manifolds.end();)
	{
		try {
			itr->A->collisionCallback(itr->B, *itr);
			itr->B->collisionCallback(itr->A, *itr);
			itr->setupManifold();
			++itr;
		}
		catch (DeletedObjectEx ex)
		{
			std::cout << ex.message << std::endl;
			Game::map->removeEntity(reinterpret_cast<Entity*>(ex.obj));
			
			Game::map->handleRoguePointer(ex.obj);
			//itr = manifolds.erase(itr);
			return;
		}
	}

	for (int i = 0; i != Game::iter; ++i)
	{
		for (auto itr = manifolds.begin(); itr != manifolds.end(); ++itr)
		{
			if (itr->A->allowCollision(itr->B, *itr) && itr->B->allowCollision(itr->A, *itr))
			{
				itr->resolveCollision();
			}
		}
	}
}

vec2i Grid::res;
std::map<Physical*, Physical*>* Grid::buckets;
std::vector<ContactManifold> Grid::manifolds;