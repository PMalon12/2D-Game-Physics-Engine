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

	//static vec2d getSupport(const std::vector<vec2d>& set, vec2d direction)
	//{
	//	float max = set[0].dot(direction);
	//	int maxI = 0;
	//	for (size_t i = 1; i < set.size(); ++i)
	//	{
	//		float dot = set[i].dot(direction);
	//		if (dot > max)
	//		{
	//			max = dot;
	//			maxI = i;
	//		}
	//	}
	//	return set[maxI];
	//}

	//static vec2d getSupport(std::vector<vec2d>& set, vec2d direction)
	//{
	//	float max = set[0].dot(direction);
	//	int maxI = 0;
	//	for (size_t i = 1; i < set.size(); ++i)
	//	{
	//		float dot = set[i].dot(direction);
	//		if (dot > max)
	//		{
	//			max = dot;
	//			maxI = i;
	//		}
	//	}
	//	return set[maxI];
	//}

	//static vec2f getSupport(Physical& A, vec2f direction)
	//{
	//	float* set;
	//	int setSize = A.getVertices(set);

	//	vec2f next(set[0], set[1]);
	//	glm::fvec4 t(next.x, next.y, 0.f, 1.f);
	//	t = A.getTransformMat() * t;

	//	float max = vec2f(t.x, t.y).dot(direction);
	//	int maxI = 0;
	//	for (size_t i = 2; i < (setSize); i += 2)
	//	{
	//		next = vec2f(set[i], set[i + 1]);
	//		t = glm::fvec4(next.x, next.y, 0.f, 1.f);
	//		t = A.getTransformMat() * t;

	//		float dot = vec2f(t.x, t.y).dot(direction);
	//		if (dot > max)
	//		{
	//			max = dot;
	//			maxI = i;
	//		}
	//	}

	//	vec2f ret(set[maxI], set[maxI + 1]);
	//	glm::fvec4 ret2(ret.x, ret.y, 0.f, 1.f);
	//	ret2 = A.getTransformMat() * ret2;
	//	ret.x = ret2.x; ret.y = ret2.y;

	//	return ret;
	//}

	//static vec2d getMinkowskiSupport(const std::vector<vec2d>& setA, const std::vector<vec2d>& setB, vec2d direction)
	//{
	//	return getSupport(setA, direction) - getSupport(setB, vec2d(0, 0) - direction);
	//}

	//static vec2f getMinkowskiSupport(std::vector<vec2d>& setA, Physical& B, vec2f direction)
	//{
	//	return getSupport(setA, direction) - getSupport(B, vec2f(0, 0) - direction);
	//}

	//static vec2f getMinkowskiSupport(Physical& A, Physical& B, vec2f direction)
	//{
	//	return getSupport(A, direction) - getSupport(B, vec2f(0, 0) - direction);
	//}



	//static CollisionInfo getClosestEdge(Convex& a, Convex& b, Simplex simplex)
	//{
	//	std::vector<vec2d> setA; setA.reserve(10); a.getVertices(setA);
	//	std::vector<vec2d> setB; setB.reserve(10); b.getVertices(setB);

	//	CollisionInfo colInfo;

	//	Polytope poly(simplex);

	//	while (true)
	//	{
	//		auto e = poly.findClosestToOrigin();

	//		vec2d p = getMinkowskiSupport(setA, setB, e.n);

	//		double d = p.dot(e.n);

	//		//std::cout << d - e.d << std::endl;

	//		if (d - e.d < 1.1)
	//		{
	//			colInfo.depth = d;
	//			colInfo.normal = e.n;
	//			return colInfo;
	//		}
	//		else
	//		{
	//			//if (std::find(poly.points.begin(), poly.points.end(), p) != poly.points.end())
	//			//{
	//			//	//std::cout << "ERROR: " << poly.points.size() << std::endl;
	//			//}
	//			poly.insert(p, e.i);
	//		}
	//	}
	//}

	//static CollisionInfo getClosestEdge(Convex& a, Physical& b, Simplex simplex)
	//{
	//	std::vector<vec2d> setA; setA.reserve(10); a.getVertices(setA);
	//	//std::vector<vec2d> setB; setB.reserve(10); b.getVertices(setB);

	//	CollisionInfo colInfo;

	//	Polytope poly(simplex);

	//	while (true)
	//	{
	//		Polytope::Edge e = poly.findClosestToOrigin();

	//		vec2d p = getMinkowskiSupport(setA, b, e.n);

	//		double d = p.dot(e.n);

	//		//std::cout << d - e.d << std::endl;

	//		if (d - e.d < 0.001)
	//		{
	//			colInfo.depth = d;
	//			colInfo.normal = e.n;
	//			return colInfo;
	//		}
	//		else
	//		{
	//			//if (std::find(poly.points.begin(), poly.points.end(), p) != poly.points.end())
	//			//{
	//			//	//std::cout << "ERROR: " << poly.points.size() << std::endl;
	//			//}
	//			poly.insert(p, e.i);
	//		}
	//	}
	//}
	
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