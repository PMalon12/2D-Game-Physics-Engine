#pragma once
#include "Include.h"
#include "Simplex.h"
#include "Polygon.h"
#include "Polytope.h"
#include "Physical.h"

class EPA //EXPANDING POLYTOPE ALGORITHM
{
public:
	EPA() {}
	~EPA() {}
	struct CollisionInfo
	{
		Polytope poly;

		vec2d normal;
		double depth;
	};

	static CollisionInfo getClosestEdge(Physical& a, Physical& b, Simplex simplex)
	{
		CollisionInfo colInfo;
		Polytope poly(simplex);

		while (true)
		{
			auto e = poly.findClosestToOrigin();

			int aInd, bInd;
			vec2f aSup = a.getSupportPoint(e.mdEdge.normal, aInd);
			vec2f bSup = b.getSupportPoint(vec2f(0, 0) - e.mdEdge.normal, bInd);

			vec2f mdSup = aSup - bSup;

			float d = mdSup.dot(e.mdEdge.normal);

			float dif = d - e.mdEdge.depth;

			if (dif < 0.001f)
			{
				colInfo.depth = d;
				colInfo.normal = e.mdEdge.normal.normalise();
				colInfo.poly = poly;
				return colInfo;
			}
			else
			{
				//if (std::find(poly.points.begin(), poly.points.end(), p) != poly.points.end())
				//{
				//	//std::cout << "ERROR: " << poly.points.size() << std::endl;
				//}
				//poly.insert(p, e.i);

				poly.insert(mdSup, aSup, bSup, e.mdEdge.index, aInd, bInd, e.mdEdge.normal, e.mdEdge.normal.negative());
			}
		}
	}


private:

};