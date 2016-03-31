//#pragma once
//#include "Include.h"
//#include "Composite Bezier.h"
//
//class CubicBezier : public Drawable
//{
//public:
//	CubicBezier();
//	~CubicBezier();
//
//	vec2f* getPoints();
//
//	void movePoint(char pPoint, vec2f pTranslation);
//	void setPoint(char pPoint, vec2f pPosition);
//
//	vec2f getPoint(double t) const;
//
//	vec2f rotate(vec2f pOrigin, vec2f pPoint, float pRotation) const;
//
//	vec2f addInDir(vec2f pOrigin, float pAdd, float pDir) const;
//
//	float getPerpendicular(vec2f pPoint1, vec2f pPoint2) const;
//
//	void draw();
//
//private:
//
//	void updateCurve();
//
//	sf::VertexArray curve;
//	VertexArray dirLines;
//	vec2f points[4];
//
//};
//
//struct ControlAnchor : public Drawable
//{
//	ControlAnchor(vec2f pAnchor, std::pair<vec2f*, vec2f*> pControls);
//	~ControlAnchor();
//
//	void draw();
//
//	vec2f* getOtherPoint(vec2f* pPointPtr);
//
//	vec2f anchor;
//	std::pair<vec2f*, vec2f*> controlsOffsets;
//};
//
//class CompositeCubicBezier : public CompositeBezierCurve
//{
//public:
//	CompositeCubicBezier();
//	CompositeCubicBezier(ControlAnchor* pAnchor);
//	~CompositeCubicBezier();
//
//	void draw();
//
//	vec2f getPoint(double t) const;
//
//	void addAnchor(ControlAnchor* pAnchor);
//	void deleteAnchor(ControlAnchor* pAnchor);
//
//	void setAnchorPosition(ControlAnchor* pAnchor, vec2f pPosition);
//
//	void setBothControlPositions(ControlAnchor* pAnchor, vec2f* pPointPtr, vec2f pPos);
//	void setSingleControlPosition(ControlAnchor* pAnchor, vec2f* pPointPtr, vec2f pPos);
//
//	ControlAnchor* getClosestAnchor(vec2f searchPosition);
//	ControlAnchor* getAnchorFromChainNumber(unsigned int chainNumber);
//
//	void linkToStart();
//
//private:
//
//	void updateBeziers();
//	void resetBeziers();
//
//	std::vector<CubicBezier*> cubicBeziers;
//
//	std::vector<ControlAnchor*> controlAnchors;
//
//	std::vector<ControlAnchor*>::iterator getAnchorIterator(ControlAnchor* pAnchor);
//
//};
