#pragma once
#include "ray.h"
#include "Game.h"

#define CrossProd(v,w) float((v.x * w.y) - (v.y * w.x))

Ray::Ray()
{

}

Ray::Ray(vec2f pSPos, vec2f pEPOffset, float pMag) : sPos(pSPos),
ePOffset(pEPOffset),
magnitude(pMag)
{

}

Ray::~Ray()
{

}

void Ray::draw()
{
	Shader* shader = Game::shaderManager.getShader("min");
	shader->use();
	auto col = glGetUniformLocation(shader->getID(), "colour");
	glUniform4f(col, 1.0f, 1.f, 1.f, 1.0f);

	glBegin(GL_LINES);

	glVertex3f(sPos.x, sPos.y, 0.0);
	glVertex3f(ePOffset.x + sPos.x, ePOffset.y + sPos.y, 0.0);

	glEnd();
}

bool Ray::doesIntersect(Ray& pRay)
{
	vec2f v = vec2f(sPos.x - pRay.sPos.x, sPos.y - pRay.sPos.y);
	vec2f w = vec2f(pRay.sPos.x - sPos.x, pRay.sPos.y - sPos.y);
	if (CrossProd(pRay.ePOffset, ePOffset) == 0)
	{
		return false;
	}
	else
	{
		float thisMag = (CrossProd(v, ePOffset)) / (CrossProd(pRay.ePOffset, ePOffset));

		float segMag = (CrossProd(w, pRay.ePOffset)) / (CrossProd(ePOffset, pRay.ePOffset));

		if (thisMag >= 0 && thisMag <= 1 && segMag >= 0 && segMag <= 1)
		{
			return true;
		}
	}
	return false;
}

bool Ray::hasCommonVertex(Ray& pRay)
{
	return ((sPos			== pRay.sPos) ||
			(sPos + ePOffset == pRay.sPos) ||
			(sPos			== pRay.sPos + pRay.ePOffset) ||
			(sPos + ePOffset == pRay.sPos + pRay.ePOffset));
}

Ray::Property Ray::checkIntersection(Ray& pRay)
{
	vec2f v = vec2f(sPos.x - pRay.sPos.x, sPos.y - pRay.sPos.y);
	vec2f w = vec2f(pRay.sPos.x - sPos.x, pRay.sPos.y - sPos.y);

	if (CrossProd(pRay.ePOffset, ePOffset) == 0)
	{
		if (CrossProd(v, pRay.ePOffset) == 0)
		{
			return Intersecting; // COLLINEAR
		}
		else
		{
			return Parallel;
		}
	}
	else
	{
		pRay.magnitude = (CrossProd(v, ePOffset)) / (CrossProd(pRay.ePOffset, ePOffset));

		float segMag = (CrossProd(w, pRay.ePOffset)) / (CrossProd(ePOffset, pRay.ePOffset));

		this->magnitude = (CrossProd(w, pRay.ePOffset)) / (CrossProd(ePOffset, pRay.ePOffset));

		if (pRay.magnitude >= 0 && pRay.magnitude <= 1 && segMag >= 0 && segMag <= 1)
		{
			pRay.ePOffset = vec2f(pRay.ePOffset.x * pRay.magnitude, pRay.ePOffset.y * pRay.magnitude);
			return Intersecting;
		}
	}
	return NonIntersecting;
}

Ray::Property Ray::checkIntersectionConst(Ray& pRay)
{
	vec2f v = vec2f(sPos.x - pRay.sPos.x, sPos.y - pRay.sPos.y);
	vec2f w = vec2f(pRay.sPos.x - sPos.x, pRay.sPos.y - sPos.y);

	if (CrossProd(pRay.ePOffset, ePOffset) == 0)
	{
		if (CrossProd(v, pRay.ePOffset) == 0)
		{
			return Intersecting; // COLLINEAR
		}
		else
		{
			return Parallel;
		}
	}
	else
	{
		float mag = pRay.magnitude;
		mag = (CrossProd(v, ePOffset)) / (CrossProd(pRay.ePOffset, ePOffset));

		float segMag = (CrossProd(w, pRay.ePOffset)) / (CrossProd(ePOffset, pRay.ePOffset));

		if (mag >= 0 && mag <= 1 && segMag >= 0 && segMag <= 1)
		{
			return Intersecting;
		}
	}
	return NonIntersecting;
}

std::pair<bool,vec2f> Ray::intersectList(std::vector<Ray>& pRays)
{
	Ray shortest(*this);
	shortest.magnitude = 100.f;

	bool upd = false;

	vec2f norm(0, 0);

	for (auto itr = pRays.begin(); itr != pRays.end(); ++itr)
	{
		//this->magnitude = 1;

		//auto res = itr->checkIntersection((*this));
		auto res = this->checkIntersectionConst((*itr));

		if (res == Ray::Property::Intersecting)
		{
			if (this->magnitude < shortest.magnitude)
			{
				upd = true;
				shortest = *this;
				norm = itr->ePOffset;
			}
			//this->magnitude = 1;
		}
	}

	*this = shortest;

	//std::cout << shortest.ePOffset.y << std::endl;

	return std::make_pair(upd,norm.normal().normalise());
}

#undef CrossProd
