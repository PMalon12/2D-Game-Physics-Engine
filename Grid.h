#pragma once
#include "Include.h"
#include "Manifold.h"

class Physical;

class Grid
{
public:
	Grid() 
	{
		size = vec2f(10000, 10000);
		resolution = vec2i(10, 10);

		cellSize.x = size.x / resolution.x;
		cellSize.y = size.y / resolution.y;
		res = resolution;
		buckets = new std::map<Physical*, Physical*>[resolution.x * resolution.y];
	}
	Grid(vec2f pSize, vec2i pResolution) : size(pSize), resolution(pResolution)
	{
		cellSize.x = size.x / resolution.x;
		cellSize.y = size.y / resolution.y;
		res = resolution;
		buckets = new std::map<Physical*, Physical*>[resolution.x * resolution.y];
	}
	~Grid() 
	{
	}

	struct ObjectBucketIDs
	{
		int fx, lx, fy, ly;
	};

	void clear() 
	{
		for (int i = 0; i < resolution.x * resolution.y; ++i)
		{
			buckets[i].clear();
		}
		manifolds.clear();
	}

	void removeObject(Physical* obj);

	void drawBounds();

	void updateBucketsForObject(Physical* obj);

	void resolveManifolds();

	static std::map<Physical*, Physical*>* buckets;
	static std::vector<ContactManifold> manifolds;

	//static std::vector<std::function<void(Physical*)>> resolveQueue;

	vec2f size;
	vec2i resolution;

	vec2f cellSize;
	static vec2i res;

};