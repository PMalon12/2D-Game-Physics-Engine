#pragma once
#include "Include.h"


class Colour
{
public:
	Colour() {}
	Colour(float pR, float pG, float pB, float pA = 1.f) : r(pR) , g(pG) , b(pB) , a(pA) {}
	Colour(double pR, double pG, double pB, double pA = 1.0) : r(pR), g(pG), b(pB), a(pA) {}
	Colour(int pR, int pG, int pB, int pA = 255) : r(float(pR) / 255.f), g(float(pG) / 255.f), b(float(pB) / 255.f), a(float(pA) / 255.f) {}
	//Colour(float pR, float pG, float pB, float pA) : r(pR), g(pG), b(pB), a(pA)  {}
	~Colour() {}

	float r, g, b, a;

	bool operator==(const Colour& rhs)
	{
		return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a));
	}

};