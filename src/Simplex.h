#pragma once
#include "Include.h"

class Simplex
{
public:
	Simplex()// : top(&(*points.begin())) 
	{
		//points.reserve(3);
		//points.resize(3);
		points[0] = vec2d(0, 0);
		points[1] = vec2d(0, 0);
		points[2] = vec2d(0, 0);
		//top = (&(*points.begin()));
		top = 0;
		size = 0;
		//points[3] = vec2f(0, 0);
		pad = vec2d(0, 0);
	}
	~Simplex() {}

	vec2d tripleProd(vec2d a, vec2d b, vec2d c) { return (b*(c.dot(a))) - (a*(c.dot(b))); }
	void push(vec2d value)
	{
		if (top < 2)
		{
			points[top] = value;
			top++;
		}
		else
		{
			points[2] = value;
		}
		return; 
	}

	void push(vec2d value, vec2d aa, vec2d bb)
	{
		if (top < 2)
		{
			//if (top == &points[0])
			//{
			//	a[0] = aa;
			//	b[0] = bb;
			//}
			//else if(top == &points[1])
			//{
			//	a[1] = aa;
			//	b[1] = bb;
			//}
			//else
			//{
			//	a[2] = aa;
			//	b[2] = bb;
			//}
			a[top] = aa;
			b[top] = bb;

			//*top = value;
			points[top] = value;
			top++;
			size++;
		}
		else
		{
			points[2] = value;
			a[2] = aa;
			b[2] = bb;
			size = 3;
		}
		return;
	}

	void push(vec2d value, vec2d aa, vec2d bb, int ai, int bi)
	{
		if (top < 2)
		{
			//if (top == &points[0])
			//{
			//	a[0] = aa;
			//	b[0] = bb;
			//}
			//else if(top == &points[1])
			//{
			//	a[1] = aa;
			//	b[1] = bb;
			//}
			//else
			//{
			//	a[2] = aa;
			//	b[2] = bb;
			//}
			a[top] = aa; aInd[top] = ai;
			b[top] = bb; bInd[top] = bi;

			//*top = value;
			points[top] = value;
			top++;
			size++;
		}
		else
		{
			points[2] = value;
			a[2] = aa; aInd[2] = ai;
			b[2] = bb; bInd[2] = bi;
			size = 3;
		}
		return;
	}

	//vec2f getLast() { return *(top-1); }
	vec2d getLast() {
		return points[size-1]; 
	}
	//size_t getSize() { return top - points; }
	size_t getSize() { return size; }

	bool containsOrigin(vec2d& direction)
	{
		vec2d AO = vec2d(0, 0) - getLast();
		if (getSize() == 3)
		{
			vec2d AB = points[1] - points[2];
			vec2d AC = points[0] - points[2];
			vec2d ABPerp = tripleProd(AC, AB, AB);
			vec2d ACPerp = tripleProd(AB, AC, AC);
			if (ABPerp.dot(AO) > 0)
			{
				points[0] = points[1]; a[0] = a[1]; b[0] = b[1]; aInd[0] = aInd[1]; bInd[0] = bInd[1];
				points[1] = points[2]; a[1] = a[2]; b[1] = b[2]; aInd[1] = aInd[2]; bInd[1] = bInd[2];
				//points[2] = vec2f(0, 0); a[2] = vec2f(0, 0); b[2] = vec2f(0, 0);
				//top = &points[2];
				top = 2; size = 2;
				direction = ABPerp.normalise();
			}
			else if (ACPerp.dot(AO) > 0)
			{
				points[1] = points[2]; a[1] = a[2]; b[1] = b[2]; aInd[1] = aInd[2]; bInd[1] = bInd[2];
				//points[2] = vec2f(0, 0); a[2] = vec2f(0, 0); b[2] = vec2f(0, 0);
				//top = &points[2];
				top = 2;
				top = 2; size = 2;
				direction = ACPerp.normalise();
			}
			else
			{
				return true;
			}
		}
		else
		{
			vec2d AB = points[0] - points[1];
			vec2d ABPerp = tripleProd(AB, AO, AB);
			if (ABPerp.magSquared() == 0)
			{
				direction = AB.getNormal();
			}
			else
			{
				direction = ABPerp.normalise();
			}
		}
		return false;
	}

	void draw()
	{
		glBegin(GL_TRIANGLES);

		glVertex3f(points[0].x, points[0].y, 0);
		glVertex3f(points[1].x, points[1].y, 0);
		glVertex3f(points[2].x, points[2].y, 0);

		glEnd();
	}

	//vec2f* top;
	int top;
	int size;
	vec2d points[3];
	//std::vector<vec2f> points;

	vec2d a[3]; int aInd[3];
	vec2d b[3]; int bInd[3];

	vec2d pad;
};