//#pragma once
//#include "Paths.h"
//#include "Game.h"
//
//vec2f BezierPath::getNextPathPoint(double pElapsedTime)
//{
//	if (progressing)
//	{
//		switch (onEnd)
//		{
//		case(Repeat) :
//		{
//			pathProgress += (pElapsedTime / timeToFinish) * ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));
//			pathProgress -= std::floor(pathProgress);
//			break;
//		}
//		case(Reverse) :
//		{
//			switch (direction)
//			{
//			case(true) : //FORWARD
//			{
//				pathProgress += (pElapsedTime / timeToFinish) * ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));
//				if (pathProgress > 1.0)
//				{
//					pathProgress = 1.0;
//					direction = false;
//				}
//				break;
//			}
//			case(false) : //BACKWARD
//			{
//				pathProgress -= (pElapsedTime / timeToFinish) * ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));
//				if (pathProgress < 0.0)
//				{
//					pathProgress = 0.0;
//					direction = true;
//				}
//				break;
//			}
//			}
//			break;
//		}
//		case(Stop) :
//		{
//			progressing = false;
//			break;
//		}
//		case(CooldownReverse) :
//		{
//			if (cdTimer > 0.0)
//			{
//				cdTimer -= Game::tickCounter.getElapsedTime();
//				break;
//			}
//
//			switch (direction)
//			{
//			case(true) : //FORWARD
//			{
//				pathProgress += (pElapsedTime / timeToFinish) * ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));
//				if (pathProgress > 1.0)
//				{
//					pathProgress = 1.0;
//					direction = false;
//					cdTimer = cooldown;
//				}
//				break;
//			}
//			case(false) : //BACKWARD
//			{
//				pathProgress -= (pElapsedTime / timeToFinish) * ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));
//				if (pathProgress < 0.0)
//				{
//					pathProgress = 0.0;
//					direction = true;
//					cdTimer = cooldown;
//				}
//				break;
//			}
//			}
//			break;
//		}
//		case(CooldownRepeat) :
//		{
//			if (cdTimer > 0.0)
//			{
//				cdTimer -= Game::tickCounter.getElapsedTime();
//				break;
//			}
//
//			pathProgress += (pElapsedTime / timeToFinish) * ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));
//
//			if (pathProgress > 1.0)
//			{
//				pathProgress = pathProgress - 1.0;
//				cdTimer = cooldown;
//			}
//			break;
//		}
//		}
//	}
//	vec2f point = bezierCurve->getPoint(pathProgress);
//	return point;
//}
//
//vec2f EllipsePath::getNextPathPoint(double pElapsedTime)
//{
//	double fractionOfFinTime = (pElapsedTime / timeToFinish / 1000000);
//	double smoothing = ((sin(pathProgress*PI)*sin(pathProgress*PI) * smoothMove) + (1.0 - smoothMove));//SOMETHING WRONG HERE, IF SMOOTH > 1 THEN MOVES BACKWARDS
//	double progressMade = fractionOfFinTime * smoothing;
//
//	if (progressing)
//	{
//		switch (onEnd)
//		{
//		case(Repeat) :
//		{
//			pathProgress += progressMade;
//			pathProgress -= std::floor(pathProgress);
//
//			break;
//		}
//		case(Reverse) :
//		{
//			switch (direction)
//			{
//			case(true) : //FORWARD
//			{
//				pathProgress += progressMade;
//				if (pathProgress > 1.0)
//				{
//					pathProgress = 1.0;
//					direction = false;
//				}
//				break;
//			}
//			case(false) : //BACKWARD
//			{
//				pathProgress -= progressMade;
//				if (pathProgress < 0.0)
//				{
//					pathProgress = 0.0;
//					direction = true;
//				}
//				break;
//			}
//			}
//			break;
//		}
//		case(Stop) :
//		{
//			progressing = false;
//			break;
//		}
//		case(CooldownReverse) :
//		{
//			if (cdTimer > 0.0)
//			{
//				cdTimer -= Game::tickCounter.getElapsedTime();
//				break;
//			}
//
//			switch (direction)
//			{
//			case(true) : //FORWARD
//			{
//				pathProgress += progressMade;
//				if (pathProgress > 1.0)
//				{
//					pathProgress = 1.0;
//					direction = false;
//					cdTimer = cooldown;
//				}
//				break;
//			}
//			case(false) : //BACKWARD
//			{
//				pathProgress -= progressMade;
//				if (pathProgress < 0.0)
//				{
//					pathProgress = 0.0;
//					direction = true;
//					cdTimer = cooldown;
//				}
//				break;
//			}
//			}
//			break;
//		}
//		case(CooldownRepeat) :
//		{
//			if (cdTimer > 0.0)
//			{
//				cdTimer -= Game::tickCounter.getElapsedTime();
//				break;
//			}
//
//			pathProgress += progressMade;
//
//			if (pathProgress > 1.0)
//			{
//				pathProgress = pathProgress - 1.0;
//				cdTimer = cooldown;
//			}
//			break;
//		}
//		}
//	}
//	vec2f point = bezierCurve->getPoint(pathProgress);
//	return point;
//}
