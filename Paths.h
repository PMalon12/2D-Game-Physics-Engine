//#pragma once
//#include "Include.h"
//#include "Beziers.h"
//
//class Pathable;
//
//class Path
//{
//public:
//	Path() : onEnd(Repeat), pathProgress(0.0), direction(true), smoothMove(0.0), timeToFinish(1.0), progressing(true) {}
//	~Path() {}
//
//	enum OnEnd { Repeat, Reverse, Stop, CooldownReverse, CooldownRepeat };
//	enum Type { CubicBezierPath, LinearPath, EllipticalPath };
//
//	virtual vec2f getNextPathPoint(double pElapsedTime) = 0;
//
//
//
//	double getPathProgress()
//	{
//		return pathProgress;
//	}
//
//	Type getPathType()
//	{
//		return pathType;
//	}
//
//	bool getProgressing() { return progressing; }
//	void setProgressing(bool pProgressing) { progressing = pProgressing; }
//
//#ifdef DrawPaths
//
//	virtual void drawPath(sf::RenderTarget& target, sf::RenderStates states) = 0;
//
//#endif DrawPaths
//
//protected:
//
//	bool progressing;
//
//	Pathable* pathable;
//
//	Type pathType;
//
//	OnEnd onEnd;
//
//	double pathProgress;
//
//	bool direction;
//
//	double smoothMove;
//
//	double timeToFinish;
//
//	double cooldown;
//	double cdTimer;
//};
//
//class BezierPath : public Path
//{
//public:
//	BezierPath(CompositeBezierCurve* pCurve) : bezierCurve(pCurve) {}
//	~BezierPath() { delete bezierCurve; }
//
//	vec2f getNextPathPoint(double pElapsedTime);
//	//NORMALIZE MOV STEP, AS FOLLOWING LONGER COMPONENT CURVES IS VISIBLY FASTER THAN SHORTER ONES
//
//#ifdef DrawPaths
//
//	void drawPath(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default)
//	{
//		bezierCurve->draw(target, states);
//	}
//
//#endif DrawPaths
//
//private:
//
//	CompositeBezierCurve* bezierCurve;
//
//};
//
//class EllipsePath : public Path
//{
//public:
//	EllipsePath(vec2f pPos, float pRadius)
//	{
//		bezierCurve = new CompositeCubicBezier();
//		bezierCurve->addAnchor(new ControlAnchor(vec2f(pPos.x, pPos.y - pRadius), std::pair<vec2f*, vec2f*>(new vec2f(pPos.x - (pRadius*0.55191502449), pPos.y - pRadius), new vec2f(pPos.x + (pRadius*0.55191502449), pPos.y - pRadius))));
//		bezierCurve->addAnchor(new ControlAnchor(vec2f(pPos.x + pRadius, pPos.y), std::pair<vec2f*, vec2f*>(new vec2f(pPos.x + pRadius, pPos.y - (pRadius*0.55191502449)), new vec2f(pPos.x + pRadius, pPos.y + (pRadius*0.55191502449)))));
//		bezierCurve->addAnchor(new ControlAnchor(vec2f(pPos.x, pPos.y + pRadius), std::pair<vec2f*, vec2f*>(new vec2f(pPos.x + (pRadius*0.55191502449), pPos.y + pRadius), new vec2f(pPos.x - (pRadius*0.55191502449), pPos.y + pRadius))));
//		bezierCurve->addAnchor(new ControlAnchor(vec2f(pPos.x - pRadius, pPos.y), std::pair<vec2f*, vec2f*>(new vec2f(pPos.x - pRadius, pPos.y + (pRadius*0.55191502449)), new vec2f(pPos.x - pRadius, pPos.y - (pRadius*0.55191502449)))));
//		bezierCurve->linkToStart();
//
//	}
//	~EllipsePath() {}
//
//	vec2f getNextPathPoint(double pElapsedTime);
//	//NORMALIZE MOV STEP, AS FOLLOWING LONGER COMPONENT CURVES IS VISIBLY FASTER THAN SHORTER ONES
//
//#ifdef DrawPaths
//
//	void drawPath(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default)
//	{
//		bezierCurve->draw(target, states);
//	}
//
//#endif DrawPaths
//
//	CompositeCubicBezier* bezierCurve;
//};
//
///*
//class LinearPath : public Path
//{
//public:
//LinearPath() {}
//~LinearPath() {}
//
//vec2f getNextPathPoint()
//{
//
//}
//};
//*/
