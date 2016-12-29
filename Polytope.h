#pragma once
#include "Simplex.h"

class Polytope
{
public:
	Polytope() {}
	Polytope(const Simplex& pSimplex)
	{
		points.push_back(Points(pSimplex.points[0], pSimplex.a[0], pSimplex.b[0], pSimplex.aInd[0], pSimplex.bInd[0]));
		points.push_back(Points(pSimplex.points[1], pSimplex.a[1], pSimplex.b[1], pSimplex.aInd[1], pSimplex.bInd[1]));
		points.push_back(Points(pSimplex.points[2], pSimplex.a[2], pSimplex.b[2], pSimplex.aInd[2], pSimplex.bInd[2]));
	}
	~Polytope() {}

	vec2d tripleProd(vec2d a, vec2d b, vec2d c) { return (b*(a.dot(c))) - (a*(b.dot(c))); }

	struct MDEdge
	{
		MDEdge() : depth(DBL_MAX) {}
		MDEdge(int pI, vec2d pN, double pD) : index(pI), normal(pN), depth(pD) {}
		int index; 
		vec2d normal;
		double depth;
	};

	struct ShapeEdge
	{
		ShapeEdge() {}
		ShapeEdge(vec2f aa, vec2f bb) : a(aa), b(bb) {}
		ShapeEdge(vec2f aa, vec2f bb, int aii, int bii) : a(aa), b(bb), ai(aii), bi(bii) {}
		ShapeEdge(vec2f aa, vec2f bb, int aii, int bii, vec2f as, vec2f bs) : a(aa), b(bb), ai(aii), bi(bii), aSupDir(as), bSupDir(bs) {}
		vec2f a, b;
		int ai, bi;
		vec2f aSupDir, bSupDir;
	};

	//void insert(vec2f point, Edge* updating) 
	//{
	//	points.push_back(point);
	//	vec2f temp = updating->offset;
	//	updating->offset = points.back() - point;
	//	edges.push_back(Edge(points.size(), points.data()[updating->a] + updating->offset - point));
	//}

	void insert(vec2d point, int index)
	{
		//points.insert(points.begin() + index , point);
	}

	struct Points
	{
		Points(vec2d c, vec2f aa, vec2f bb) : mdPoint(c), aPoint(aa), bPoint(bb) {}
		Points(vec2d c, vec2f aa, vec2f bb, int aai, int bbi) : mdPoint(c), aPoint(aa), bPoint(bb), aIndex(aai), bIndex(bbi) {}
		vec2d mdPoint;
		vec2f aPoint; int aIndex; vec2f aSuppDir;
		vec2f bPoint; int bIndex; vec2f bSuppDir;
	};

	void insert(vec2d point, vec2f a, vec2f b, int index)
	{
		points.insert(points.begin() + index, Points(point, a, b));
	}

	void insert(vec2d point, vec2f a, vec2f b, int index, int ai, int bi, vec2f aSupDir, vec2f bSupDir)
	{
		points.insert(points.begin() + index, Points(point, a, b, ai, bi));
	}
	
	struct ContactEdges
	{
		ContactEdges() {}
		ContactEdges(ShapeEdge aa, ShapeEdge bb, MDEdge cc) : aEdge(aa), bEdge(bb), mdEdge(cc) {}
		ShapeEdge aEdge;
		ShapeEdge bEdge;
		MDEdge mdEdge;
	};

	ContactEdges findClosestToOrigin()
	{
		MDEdge closestEdge;
		ShapeEdge edgeA;
		ShapeEdge edgeB;

		for (int i = 0; i < points.size(); ++i)
		{
			int j = i + 1 == points.size() ? 0 : i + 1;

			vec2d a = points[i].mdPoint;
			vec2d b = points[j].mdPoint;

			vec2d e = b - a;

			vec2d oa = a;

			//vec2f n = tripleProd(e, oa, e);
			vec2d n = vec2d(-e.y, e.x);

			n.normalise();

			float d = n.dot(b);

			if (d < closestEdge.depth)
			{
				edgeA.a = points[i].aPoint; edgeA.ai = points[i].aIndex; edgeA.aSupDir = points[i].aSuppDir;
				edgeA.b = points[j].aPoint; edgeA.bi = points[j].aIndex; edgeA.bSupDir = points[j].aSuppDir;

				edgeB.a = points[i].bPoint; edgeB.ai = points[i].bIndex; edgeB.aSupDir = points[i].bSuppDir;
				edgeB.b = points[j].bPoint; edgeB.bi = points[j].bIndex; edgeB.bSupDir = points[j].bSuppDir;

				closestEdge.depth = d;
				closestEdge.normal = n;
				closestEdge.index = j;
			}

			//if (points.size() > 100)
			//{
			//	std::cout << ">100" <<  std::endl;
			//	contactEdges = ContactEdges(edgeA, edgeB, closestEdge);
			//	return contactEdges;
			//}
		}

		if (closestEdge.index < 0)
		{
			std::cout << std::endl;
		}

		contactEdges = ContactEdges(edgeA, edgeB, closestEdge);

		return contactEdges;
	}


	ContactEdges contactEdges;
	//std::vector<vec2d> points;
	std::vector<Points> points;
	std::list<MDEdge> edges;

private:
};