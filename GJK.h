#pragma once
#include "Include.h"
#include "Simplex.h"

class GJK //GILBERT-JOHNSON-KEERTHI
{
public:
	GJK() {}
	~GJK() {}

	//static vec2f getSupport(const std::vector<vec2f>& set, vec2f direction)
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
	//static vec2f getSupport(float* set, unsigned int numTri , vec2f direction)
	//{
	//	float max = vec2f(set[0],set[1]).dot(direction);
	//	int maxI = 0;
	//	for (size_t i = 2; i < (numTri); i+=2)
	//	{
	//		float dot = vec2f(set[i],set[i+1]).dot(direction);
	//		if (dot > max)
	//		{
	//			max = dot;
	//			maxI = i;
	//		}
	//	}
	//	return vec2f(set[maxI],set[maxI+1]);
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

	static std::pair<bool, Simplex> checkCollision(Physical& a, Physical& b)
	{
		vec2d searchDir = b.getBodyPosition() - a.getBodyPosition();
		Simplex s;
		
		int aInd, bInd;
		vec2f aSup = a.getSupportPoint(searchDir, aInd);
		vec2f bSup = b.getSupportPoint(searchDir.negative(), bInd);
		s.push(aSup - bSup, aSup, bSup, aInd, bInd);
		
		searchDir.negate();
		while (true)
		{
			int aInd, bInd;
			vec2d aSup = a.getSupportPoint(searchDir, aInd);
			vec2d bSup = b.getSupportPoint(searchDir.negative(), bInd);
			s.push(aSup - bSup, aSup, bSup, aInd, bInd);

			double dot = s.getLast().dot(searchDir);

			//std::cout << dot << std::endl;

			if (dot <= 0.1)
			{
				return std::make_pair(false, s);
			}
			else if (s.containsOrigin(searchDir))
			{
				return std::make_pair(true, s);
			}
		}
	}

};