#pragma once
#include "Vector.h"
#include "State.h"
#include "Transform.h"
#include <exception>
#include <functional>
#include "Include.h"

class RigidBody
{
public:
	RigidBody() : staticFriction(0.2f), dynamicFriction(0.1f), restitution(0.0f), pushBackStrength(0.2f) {}
	~RigidBody() {}

	inline void applyLinearForce(vec2f force)
	{
		state.momentum += force;
		state.velocity = state.momentum * state.inverseMass;
	}

	inline void applyForce(vec2f force, vec2f relForcePos)
	{
		state.angularMomentum += (relForcePos).cross(force);
		state.angularVelocity = state.angularMomentum * state.inverseInertiaMoment;

		state.momentum += force;
		state.velocity = state.momentum * state.inverseMass;
	}

	inline void applyTorque(vec2f force, vec2f forcePos)
	{
		state.angularMomentum += (force).cross((state.position - forcePos) * PIXTOMET);
		state.angularVelocity = state.angularMomentum * state.inverseInertiaMoment;
	}

	inline void applyLinearImpulse(vec2f force, float time)
	{
		tForces.push_back(TimedForce(force, time));
	}

	void updateRigidBody(float dt, Transformable& transform, std::function<vec2f(State&, float)> force, std::function<float(State&, float)> torque);

	void setBodyPosition(vec2f pPos) { state.position = pPos; }
	vec2f getBodyPosition() {
		return state.position; 
	}
	void warpBody(vec2f pWarp) { state.position += pWarp; }

	void setBodyVelocity(vec2f pVel) { state.velocity = pVel; state.momentum = pVel * state.mass; }
	vec2f getBodyVelocity() { return state.velocity; }

	void setBodyMomentum(vec2f pMom) { state.momentum = pMom; }
	vec2f getBodyMomentum() { return state.momentum; }

	void setBodyRotation(float pRot) { state.rotation = pRot; }
	float getBodyRotation() { return state.rotation; }

	void setBodyAngularVelocity(float pAngV) { state.angularVelocity = pAngV; }
	float getBodyAngularVelocity() { return state.angularVelocity; }

	void setBodyAngularMomentum(float pAngP) { state.angularMomentum = pAngP; }
	float getBodyAngularMomentum() { return state.angularMomentum; }

	void setBodyMass(float pMass) {
		if (pMass <= 0) { throw std::exception("Body mass set to <= 0"); };
		state.mass = pMass;
		state.inverseMass = 1.f / pMass;
		if (pMass == FLT_MAX)
		{
			state.inverseMass = 0.f;
			affectedByGravity = false;
		}
	}
	float getBodyMass() { return state.mass; }
	float getBodyInverseMass() { return state.inverseMass; }
	
	void setBodyInertiaMoment(float pMomIn) {
		if (pMomIn <= 0) { throw std::exception("Body inertia moment set to <= 0"); };
		state.inertiaMoment = pMomIn;
		state.inverseInertiaMoment = 1.f / pMomIn;
		if (pMomIn == FLT_MAX)
		{
			state.inverseInertiaMoment = 0.f;
		}
	}
	float getBodyInertiaMoment() { return state.inertiaMoment; }
	float getBodyInverseInertiaMoment() { return state.inverseInertiaMoment; }

	void setStatic() 
	{ 
		state.mass = FLT_MAX; state.inverseMass = 0.f;
		state.inertiaMoment = FLT_MAX; state.inverseInertiaMoment = 0.f;
		affectedByGravity = false;
	}

	State state;
	float staticFriction;
	float dynamicFriction;
	float restitution;
	float pushBackStrength;
	bool affectedByGravity;

private:

	struct TimedForce
	{
		TimedForce(vec2f f, float t) : force(f), timeLeft(t), totalTime(t) {}
		float totalTime;
		float timeLeft;
		vec2f force;
	};

	std::vector<TimedForce> tForces;

};