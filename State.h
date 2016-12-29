#pragma once
#include "Vector.h"

struct State
{
	State() : position(0,0), velocity(0,0), momentum(0,0), rotation(0), angularVelocity(0), angularMomentum(0), mass(1), inverseMass(1), inertiaMoment(1), inverseInertiaMoment(1), centerOfMass(0,0) {}
	vec2d position;
	vec2d velocity;
	vec2d momentum;

	double rotation;
	double angularVelocity;
	double angularMomentum;

	double mass;
	double inverseMass;

	double inertiaMoment;
	double inverseInertiaMoment;

	vec2d centerOfMass;
};

struct Derivative
{
	vec2d dx;
	double dtheta;

	vec2d dp;
	double dpang;
};
