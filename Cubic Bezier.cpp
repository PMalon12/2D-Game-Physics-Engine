//#pragma once
//#include "Cubic Bezier.h"
//#include "Game.h"
//
//// *********************************** CUBIC BEZIER //
//
//
//CubicBezier::CubicBezier()
//{
//	curve.setPrimitiveType(sf::PrimitiveType::Lines);
//	dirLines.setPrimitiveType(sf::PrimitiveType::Lines);
//}
//
//CubicBezier::~CubicBezier()
//{
//
//}
//
//vec2f * CubicBezier::getPoints()
//{
//	return points;
//}
//
//void CubicBezier::movePoint(char pPoint, vec2f pTranslation)
//{
//	points[pPoint] += pTranslation;
//	updateCurve();
//}
//
//void CubicBezier::setPoint(char pPoint, vec2f pPosition)
//{
//	points[pPoint] = pPosition;
//	updateCurve();
//}
//
//vec2f CubicBezier::getPoint(double t) const
//{
//	return vec2f(((1.0 - t) * (1.0 - t) * (1.0 - t) * points[0].x) + (3 * (1.0 - t) * (1.0 - t) * t * points[1].x) + (3.0 * (1.0 - t) * t * t * points[2].x) + (t * t * t * points[3].x),
//		((1.0 - t) * (1.0 - t) * (1.0 - t) * points[0].y) + (3 * (1.0 - t) * (1.0 - t) * t * points[1].y) + (3.0 * (1.0 - t) * t * t * points[2].y) + (t * t * t * points[3].y));
//}
//
//vec2f CubicBezier::rotate(vec2f pOrigin, vec2f pPoint, float pRotation) const
//{
//	vec2f newPoint;
//	newPoint.x = ((pPoint.x - pOrigin.x)*cosf(pRotation)) - ((pPoint.y - pOrigin.y)*sinf(pRotation)) + pOrigin.x;
//	newPoint.y = ((pPoint.x - pOrigin.x)*sinf(pRotation)) + ((pPoint.y - pOrigin.y)*cosf(pRotation)) + pOrigin.y;
//	return newPoint;
//}
//
//vec2f CubicBezier::addInDir(vec2f pOrigin, float pAdd, float pDir) const
//{
//	vec2f newPoint;
//	newPoint.x = pOrigin.x + (pAdd * cosf(pDir));
//	newPoint.y = pOrigin.y + (pAdd * sinf(pDir));
//	return newPoint;
//}
//
//float CubicBezier::getPerpendicular(vec2f pPoint1, vec2f pPoint2) const
//{
//	return atanf(((pPoint1.y - pPoint2.y) / (pPoint1.x - pPoint2.x)));
//}
//
//void CubicBezier::draw()
//{
//	states.texture = sf::RenderStates::Default.texture;
//
//	target.draw(dirLines, states);
//	target.draw(curve, states);
//}
//
//void CubicBezier::updateCurve()
//{
//	curve.clear();
//	dirLines.clear();
//	for (int i = 0; i < 20; ++i)
//	{
//		curve.append(Vertex(getPoint(double(i) * 0.05)));
//		if (i % 2 == 0)
//		{
//			vec2f pointPlus1, pointPlus2;
//			pointPlus1 = getPoint(double(i + 1) * 0.05);
//			pointPlus2 = getPoint(double(i + 2) * 0.05);
//
//			if (getPerpendicular(pointPlus2, pointPlus1) > 0)
//			{
//				dirLines.append(Vertex(pointPlus1));
//				dirLines.append(Vertex(rotate(pointPlus1, addInDir(pointPlus1, 10.f, getPerpendicular(pointPlus2, pointPlus1)), -PI / 4)));
//			}
//			else
//			{
//				dirLines.append(Vertex(pointPlus1));
//				dirLines.append(Vertex(rotate(pointPlus1, addInDir(pointPlus1, 10.f, getPerpendicular(pointPlus2, pointPlus1)), -PI / 4)));
//			}
//
//			if (hypotf(dirLines.operator[](dirLines.getVertexCount() - 1).position.x - pointPlus2.x, dirLines.operator[](dirLines.getVertexCount() - 1).position.y - pointPlus2.y) < hypot(pointPlus2.x - pointPlus1.x, pointPlus2.y - pointPlus1.y))
//			{
//				dirLines.operator[](dirLines.getVertexCount() - 1).position = rotate(pointPlus1, addInDir(pointPlus1, -10.f, getPerpendicular(pointPlus2, pointPlus1)), -PI / 4);
//			}
//
//			if (getPerpendicular(pointPlus2, pointPlus1) > 0)
//			{
//				dirLines.append(Vertex(pointPlus1));
//				dirLines.append(Vertex(rotate(pointPlus1, addInDir(pointPlus1, 10.f, getPerpendicular(pointPlus2, pointPlus1)), PI / 4)));
//			}
//			else
//			{
//				dirLines.append(Vertex(pointPlus1));
//				dirLines.append(Vertex(rotate(pointPlus1, addInDir(pointPlus1, 10.f, getPerpendicular(pointPlus2, pointPlus1)), PI / 4)));
//			}
//
//			if (hypotf(dirLines.operator[](dirLines.getVertexCount() - 1).position.x - pointPlus2.x, dirLines.operator[](dirLines.getVertexCount() - 1).position.y - pointPlus2.y) < hypot(pointPlus2.x - pointPlus1.x, pointPlus2.y - pointPlus1.y))
//			{
//				dirLines.operator[](dirLines.getVertexCount() - 1).position = rotate(pointPlus1, addInDir(pointPlus1, -10.f, getPerpendicular(pointPlus2, pointPlus1)), PI / 4);
//			}
//		}
//	}
//	curve.append(Vertex(getPoint(1)));
//}
//
//
//// *********************************** CONTROL ANCHOR //
//
//
//ControlAnchor::ControlAnchor(vec2f pAnchor, std::pair<vec2f*, vec2f*> pControls) : anchor(pAnchor), controlsOffsets(pControls)
//{
//
//}
//
//ControlAnchor::~ControlAnchor()
//{
//	delete controlsOffsets.first;
//	delete controlsOffsets.second;
//}
//
//void ControlAnchor::draw()
//{
//	VertexArray lines;
//	lines.append(Vertex(*controlsOffsets.first, sf::Color(255, 0, 0, 255)));
//	lines.append(Vertex(anchor, sf::Color(255, 0, 0, 255)));
//	lines.append(Vertex(*controlsOffsets.second, sf::Color(255, 0, 0, 255)));
//	lines.setPrimitiveType(sf::PrimitiveType::LinesStrip);
//	target.draw(lines, states);
//}
//
//vec2f * ControlAnchor::getOtherPoint(vec2f* pPointPtr)
//{
//	if (controlsOffsets.first = pPointPtr)
//	{
//		return controlsOffsets.second;
//	}
//	else
//	{
//		return controlsOffsets.first;
//	}
//}
//
//
//// *********************************** COMPOSITE CUBIC BEZIER //
//
//
//CompositeCubicBezier::CompositeCubicBezier()
//{
//
//}
//
//CompositeCubicBezier::CompositeCubicBezier(ControlAnchor* pAnchor)
//{
//	controlAnchors.push_back(pAnchor);
//}
//
//CompositeCubicBezier::~CompositeCubicBezier()
//{
//	for (auto i = cubicBeziers.begin(); i != cubicBeziers.end(); ++i)
//	{
//		delete (*i);
//	}
//
//	for (auto i = controlAnchors.begin(); i != controlAnchors.end(); ++i)
//	{
//		delete (*i);
//	}
//}
//
//void CompositeCubicBezier::draw()
//{
//	for (auto i = cubicBeziers.begin(); i != cubicBeziers.end(); ++i)
//	{
//		(*i)->draw(target, states);
//	}
//
//	sf::CircleShape startPoint, endPoint;
//
//	startPoint.setFillColor(sf::Color(0, 0, 0, 0));
//	startPoint.setOutlineColor(sf::Color(0, 50, 205, 255));
//	startPoint.setOutlineThickness(2.f);
//	startPoint.setRadius(8);
//	startPoint.setOrigin(8, 8);
//	startPoint.setPosition(getPoint(0.0));
//
//	endPoint.setFillColor(sf::Color(205, 50, 0, 255));
//	endPoint.setOutlineColor(sf::Color(120, 30, 20, 255));
//	endPoint.setOutlineThickness(1.f);
//	endPoint.setRadius(4);
//	endPoint.setOrigin(4, 4);
//	endPoint.setPosition(getPoint(0.99999));
//
//	for (auto i = controlAnchors.begin(); i != controlAnchors.end(); ++i)
//	{
//		(*i)->draw(target, states);
//	}
//
//	target.draw(startPoint, states);
//	target.draw(endPoint, states);
//}
//
//void CompositeCubicBezier::addAnchor(ControlAnchor* pAnchor)
//{
//	controlAnchors.push_back(pAnchor);
//	updateBeziers();
//}
//
//void CompositeCubicBezier::deleteAnchor(ControlAnchor* pAnchor)
//{
//	auto anchorItr = getAnchorIterator(pAnchor);
//	if (anchorItr != controlAnchors.end())
//	{
//		controlAnchors.erase(anchorItr);
//		delete pAnchor;
//	}
//	updateBeziers();
//}
//
//void CompositeCubicBezier::setAnchorPosition(ControlAnchor* pAnchor, vec2f pPosition)
//{
//	pAnchor->anchor = pPosition;
//	updateBeziers();
//}
//
//void CompositeCubicBezier::setBothControlPositions(ControlAnchor* pAnchor, vec2f* pPointPtr, vec2f pPos)
//{
//	*pPointPtr = pPos;
//	*(pAnchor->getOtherPoint(pPointPtr)) = vec2f(pPos.x - (2.f*(pPos.x - pAnchor->anchor.x)), pPos.y - (2.f*(pPos.y - pAnchor->anchor.y)));
//	updateBeziers();
//}
//
//void CompositeCubicBezier::setSingleControlPosition(ControlAnchor* pAnchor, vec2f* pPointPtr, vec2f pPos)
//{
//	*pPointPtr = pPos;
//	updateBeziers();
//}
//
//ControlAnchor* CompositeCubicBezier::getClosestAnchor(vec2f searchPosition)
//{
//	float shortestDist = 5.f;
//	float compDist;
//	ControlAnchor* closest = nullptr;
//	for (auto i = controlAnchors.begin(); i != controlAnchors.end(); ++i)
//	{
//		compDist = hypotf((searchPosition.x - (*i)->anchor.x), (searchPosition.y - (*i)->anchor.y));
//		if (compDist < shortestDist)
//		{
//			shortestDist = compDist;
//			closest = *i;
//		}
//	}
//	return closest;
//}
//
//ControlAnchor* CompositeCubicBezier::getAnchorFromChainNumber(unsigned int chainNumber)
//{
//	return controlAnchors[chainNumber];
//}
//
//std::vector<ControlAnchor*>::iterator CompositeCubicBezier::getAnchorIterator(ControlAnchor* pAnchor)
//{
//	for (auto i = controlAnchors.begin(); i != controlAnchors.end(); ++i)
//	{
//		if (*i = pAnchor)
//		{
//			return i;
//		}
//	}
//	return  controlAnchors.end();
//}
//
//vec2f CompositeCubicBezier::getPoint(double t) const
//{
//	if (t > 1.0)
//	{
//		std::cout << "COMPOSITE CUBIC BEZIER :: GET POINT -- ERROR" << std::endl;
//	}
//	double x = t * static_cast<double>(cubicBeziers.size());
//	short f = floor(x);
//	if (f < cubicBeziers.size())
//	{
//		return cubicBeziers[f]->getPoint(x - static_cast<double>(f));
//	}
//}
//
//void CompositeCubicBezier::updateBeziers()
//{
//	resetBeziers();
//	CubicBezier temp;
//	for (auto i = controlAnchors.begin(); i != (controlAnchors.end() - 1); ++i)
//	{
//		temp.setPoint(0, (*i)->anchor);
//		temp.setPoint(1, *(*i)->controlsOffsets.second);
//		temp.setPoint(2, *(*(i + 1))->controlsOffsets.first);
//		temp.setPoint(3, (*(i + 1))->anchor);
//		cubicBeziers.push_back(new CubicBezier(temp));
//	}
//}
//
//void CompositeCubicBezier::resetBeziers()
//{
//	for (auto i = cubicBeziers.begin(); i != cubicBeziers.end(); ++i)
//	{
//		delete *i;
//	}
//	cubicBeziers.clear();
//}
//
//void CompositeCubicBezier::linkToStart()
//{
//	controlAnchors.push_back(new ControlAnchor(*controlAnchors.front()));
//	updateBeziers();
//}
