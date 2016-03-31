#pragma once
#include "include.h"

class Ray
{
public:
	Ray();
	Ray(vec2f pSPos, vec2f pEPOffset, float pMag);
	~Ray();

	enum Property { NonIntersecting, Parallel, Collinear, Intersecting };

	vec2f sPos;
	vec2f ePOffset; //End point offset from start point

	void draw();

	float magnitude;

	std::pair<bool,vec2f> intersectList(std::vector<Ray>& pRays);
	Property checkIntersection(Ray& pRay);
	Property checkIntersectionConst(Ray& pRay);
	bool doesIntersect(Ray& pRay);
	bool hasCommonVertex(Ray& pRay);
};

