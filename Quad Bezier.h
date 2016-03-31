//#pragma once
//#include "Include.h"
//
//class QuadBezier : public Drawable, public sf::Transformable
//{
//public:
//	QuadBezier() {}
//	~QuadBezier() {}
//
//	vec2f * getPoints()
//	{
//		return points;
//	}
//
//	void movePoint(char pPoint, vec2f pTranslation)
//	{
//		points[pPoint] += pTranslation;
//	}
//
//	void setPoint(char pPoint, vec2f pPosition)
//	{
//		points[pPoint] = pPosition;
//	}
//
//	vec2f getPoint(float t) const
//	{
//		//return vec2f(((1.f - t)*(((1.f - t) * points[0].x) + (t * points[1].x))) + (t*(((1.f - t) * points[1].x) + (t * points[2].x))) ,
//		//					((1.f - t)*(((1.f - t) * points[0].y) + (t * points[1].y))) + (t*(((1.f - t) * points[1].y) + (t * points[2].y))));
//
//		return vec2f((pow((1.f - t), 2) * points[0].x) + (2 * (1.f - t) * t * points[1].x) + (pow(t, 2) * points[2].x),
//			(pow((1.f - t), 2) * points[0].y) + (2 * (1.f - t) * t * points[1].y) + (pow(t, 2) * points[2].y));
//	}
//
//	void draw()
//	{
//		states.transform *= getTransform();
//
//		VertexArray curve;
//		curve.setPrimitiveType(sf::PrimitiveType::LinesStrip);
//		for (int i = 0; i < 100; ++i)
//		{
//			curve.append(Vertex(getPoint(float(i) * 0.01)));
//		}
//		curve.append(Vertex(getPoint(1)));
//
//		target.draw(curve, states);
//	}
//
//private:
//
//	vec2f points[3];
//
//};
//
//class CompositeQuadBezier : public Drawable, public sf::Transformable
//{
//public:
//	CompositeQuadBezier();
//	~CompositeQuadBezier();
//
//	std::vector<QuadBezier*> quadBeziers;
//
//
//};